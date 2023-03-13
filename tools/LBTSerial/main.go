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
