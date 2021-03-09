package main

import (
	"sync"
)

/* snail queue, goroutine safe */
type snail_queue_t struct {
	lock     sync.Mutex // internal lock
	head     uint16
	tail     uint16
	capacity uint16
	elements []interface{}
}

/* Specify queue capacity initialization, max capacity 256. */
func init_snail_queue(capacity uint16) *snail_queue_t {

	var max_cap uint16 = 256

	if capacity == 0 || capacity > max_cap {
		capacity = max_cap
	}
	capacity += 1 // for next index

	return &snail_queue_t{
		elements: make([]interface{}, capacity),
		capacity: capacity,
	}
}

func (queue *snail_queue_t) is_empty() bool {

	return queue.head == queue.tail
}

func (queue *snail_queue_t) is_full() bool {

	return (queue.tail+1)%queue.capacity == queue.head
}

// When the queue is full, enqueue will fail
func (queue *snail_queue_t) enqueue(obj interface{}) bool {

	queue.lock.Lock()
	defer queue.lock.Unlock()

	if queue.is_full() {
		return false
	}
	queue.elements[queue.tail] = obj
	queue.tail = (queue.tail + 1) % queue.capacity

	return true
}

// When the queue is empty, dequeue will fail
func (queue *snail_queue_t) dequeue() (interface{}, bool) {

	queue.lock.Lock()
	defer queue.lock.Unlock()

	var obj interface{}
	var result bool = false

	if !queue.is_empty() {

		obj = queue.elements[queue.head]
		queue.head = (queue.head + 1) % queue.capacity
		result = true
	}
	return obj, result
}
