package webgui

import (
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"strconv"
	"strings"

	. "../virtualmachine"
	"github.com/zserge/webview"
)

func startServer(indexHTML string) string {
	ln, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		log.Fatal(err)
	}
	go func() {
		defer ln.Close()
		http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
			w.Write([]byte(indexHTML))
		})
		log.Fatal(http.Serve(ln, nil))
	}()
	return "http://" + ln.Addr().String()
}

func handleRPC(w webview.WebView, data string) {
	switch {
	case data == "close":
		w.Terminate()
	case data == "fullscreen":
		w.SetFullscreen(true)
	case data == "unfullscreen":
		w.SetFullscreen(false)
	case data == "open":
		log.Println("open", w.Dialog(webview.DialogTypeOpen, 0, "Open file", ""))
	case data == "opendir":
		log.Println("open", w.Dialog(webview.DialogTypeOpen, webview.DialogFlagDirectory, "Open directory", ""))
	case data == "save":
		log.Println("save", w.Dialog(webview.DialogTypeSave, 0, "Save file", ""))
	case data == "message":
		w.Dialog(webview.DialogTypeAlert, 0, "Hello", "Hello, world!")
	case data == "info":
		w.Dialog(webview.DialogTypeAlert, webview.DialogFlagInfo, "Hello", "Hello, info!")
	case data == "warning":
		w.Dialog(webview.DialogTypeAlert, webview.DialogFlagWarning, "Hello", "Hello, warning!")
	case data == "error":
		w.Dialog(webview.DialogTypeAlert, webview.DialogFlagError, "Hello", "Hello, error!")
	case strings.HasPrefix(data, "changeTitle:"):
		w.SetTitle(strings.TrimPrefix(data, "changeTitle:"))
	case strings.HasPrefix(data, "changeColor:"):
		hex := strings.TrimPrefix(strings.TrimPrefix(data, "changeColor:"), "#")
		num := len(hex) / 2
		if !(num == 3 || num == 4) {
			log.Println("Color must be RRGGBB or RRGGBBAA")
			return
		}
		i, err := strconv.ParseUint(hex, 16, 64)
		if err != nil {
			log.Println(err)
			return
		}
		if num == 3 {
			r := uint8((i >> 16) & 0xFF)
			g := uint8((i >> 8) & 0xFF)
			b := uint8(i & 0xFF)
			w.SetColor(r, g, b, 255)
			return
		}
		if num == 4 {
			r := uint8((i >> 24) & 0xFF)
			g := uint8((i >> 16) & 0xFF)
			b := uint8((i >> 8) & 0xFF)
			a := uint8(i & 0xFF)
			w.SetColor(r, g, b, a)
			return
		}
	}
}

func MakeWindow(vm *VirtualMachine) {
	html := vm.Pop()
	title := vm.Pop()
	width := vm.Pop()
	height := vm.Pop()
	switch html.(type) {
	case string:
		switch title.(type) {
		case string:
			switch width.(type) {
			case float64:
				switch height.(type) {
				case float64:
					url := startServer(html.(string))
					w := webview.New(webview.Settings{
						Width:     int(width.(float64)),
						Height:    int(height.(float64)),
						Title:     title.(string),
						Resizable: true,
						URL:       url,
						ExternalInvokeCallback: handleRPC,
					})
					w.SetColor(255, 255, 255, 255)
					defer w.Exit()
					w.Run()
				default:
					fmt.Println(errors.New("ERROR: Height must be a Num"))
					os.Exit(1)
				}
			default:
				fmt.Println(errors.New("ERROR: Width must be a Num"))
				os.Exit(1)
			}
		default:
			fmt.Println(errors.New("ERROR: Title must be a string"))
			os.Exit(1)
		}
	default:
		fmt.Println(errors.New("ERROR: HTML must be a string"))
		os.Exit(1)
	}
}

func InstallLibrary(vm *VirtualMachine) {
	vm.Library["webgui.make_window"] = MakeWindow
	vm.Push("{ webgui.make_window % }")
	vm.Push("webgui.MakeWindow")
	vm.Op_store(false)
}
