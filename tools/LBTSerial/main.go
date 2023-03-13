// MIT License

// Copyright (c) 2023 LBT_sig

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

package main

import (
    "bufio"
    "fmt"
    "log"
    "os"
    "github.com/tarm/serial"
)

func main() {
    // 打开串口设备，这里使用的是Linux系统下的/dev/ttyUSB0设备
    port, err := serial.OpenPort(&serial.Config{Name: "/dev/ttyUSB0", Baud: 115200})
    if err != nil {
        log.Fatal(err)
    }
    defer port.Close()

    // 启动一个协程用于从串口读取数据并将其显示在终端上
    go func() {
        for {
            // 从串口读取一行数据
            reader := bufio.NewReader(port)
            line, err := reader.ReadString('\n')
            if err != nil {
                log.Fatal(err)
            }

            // 显示从串口读取的数据
            fmt.Print(line)
        }
    }()

    // 启动一个协程用于从终端读取用户输入并将其发送到串口
    go func() {
        for {
            // 从终端读取一行数据
            reader := bufio.NewReader(os.Stdin)
            line, err := reader.ReadString('\n')
            if err != nil {
                log.Fatal(err)
            }

            // 将用户输入的数据发送到串口
            _, err = port.Write([]byte(line))
            if err != nil {
                log.Fatal(err)
            }
        }
    }()

    // 无限循环，让程序保持运行状态
    for {
    }
}
