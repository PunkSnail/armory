package main

import (
	"fmt"
	"time"
)

type phony_t struct {
	name     string
	describe string
	num      int
}

func test_enqueue1(queue *snail_queue_t) {

	for i := 0; i < 6; i++ {

		if !queue.enqueue(&phony_t{
			name: "snail", describe: "boy", num: i}) {
			fmt.Printf("enqueue1 is full\n")
		}
		time.Sleep(1 * time.Second)
	}
}

func test_enqueue2(queue *snail_queue_t) {

	for i := 0; i < 6; i++ {

		if !queue.enqueue(&phony_t{
			name: "liban", describe: "girl", num: i}) {
			fmt.Printf("enqueue2 is full\n")
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	queue := init_snail_queue(5)

	go test_enqueue1(queue)
	go test_enqueue2(queue)

	for i := 0; i < 7; i++ {

		for !queue.is_empty() {
			obj, _ := queue.dequeue()
			phony := obj.(*phony_t)
			fmt.Printf("%v\n", *phony)
		}
		time.Sleep(1 * time.Second)
	}
}
