package main

import (
	"fmt"
)

type phony_t struct {
	name     string
	describe string
	num      int
}

func test_enqueue1(queue *SnailQueue) {

	for i := 0; i < 6; i++ {

		if !queue.Enqueue(&phony_t{
			name: "liban", describe: "girl", num: i}) {
			fmt.Printf("the queue is full\n")
		}
	}
	for i := 0; i < 10; i++ {

		for !queue.IsEmpty() {
			obj, _ := queue.Dequeue()
			phony := obj.(*phony_t)
			fmt.Printf("%v\n", *phony)
		}
	}
}

func test_enqueue2(queue *SnailQueue) {

	for i := 0; i < 15; i++ {

		if queue.IsFull() {
			queue.Dequeue()
		}
		queue.Enqueue(i)
	}
	queue.Dequeue()

	var limit uint16 = 5

	queue.Traversal(func(ele interface{}) {
		num := ele.(int)
		fmt.Printf("num: %d\n", num)
	}, limit)

	fmt.Printf("\n")
	queue.ReverseTraversal(func(ele interface{}) {
		num := ele.(int)
		fmt.Printf("num: %d\n", num)
	}, limit)
}

func main() {

	queue := CreateSnailQueue(8)

	test_enqueue1(queue)
	test_enqueue2(queue)
}
