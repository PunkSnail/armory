package main

type TraversalCallback func(ele interface{})

/* snail queue, goroutine unsafe */
type SnailQueue struct {
	head     uint16
	tail     uint16
	size     uint16
	capacity uint16
	elements []interface{}
}

/* Specify queue capacity initialization, default capacity 256. */
func CreateSnailQueue(capacity uint16) *SnailQueue {

	if capacity == 0 {
		capacity = 256
	}
	capacity += 1 // for next index

	return &SnailQueue{
		elements: make([]interface{}, capacity),
		capacity: capacity,
	}
}

func (queue *SnailQueue) IsEmpty() bool {

	return queue.head == queue.tail
}

func (queue *SnailQueue) IsFull() bool {

	return (queue.tail+1)%queue.capacity == queue.head
}

// When the queue is full, enqueue will fail
func (queue *SnailQueue) Enqueue(obj interface{}) bool {

	if nil == queue {
		return false
	}
	if queue.IsFull() {
		return false
	}
	queue.elements[queue.tail] = obj
	queue.tail = (queue.tail + 1) % queue.capacity
	queue.size++

	return true
}

// When the queue is empty, dequeue will fail
func (queue *SnailQueue) Dequeue() (interface{}, bool) {

	var result bool = false
	var obj interface{}

	if nil == queue {
		return obj, result
	}
	if !queue.IsEmpty() {

		obj = queue.elements[queue.head]
		queue.head = (queue.head + 1) % queue.capacity
		queue.size--
		result = true
	}
	return obj, result
}

// first in first access
// traversing queue, when the limit is 0, traverse the entire queue
func (queue *SnailQueue) Traversal(cb TraversalCallback, limit uint16) {

	if nil == queue || nil == cb {
		return
	}
	if 0 == limit {
		limit = 0xFFFF
	}
	for i := queue.head; i != queue.tail; i = (i + 1) % queue.capacity {
		if 0 == limit {
			break
		} else {
			limit--
			cb(queue.elements[i])
		}
	}
}

// first in last access
func (queue *SnailQueue) ReverseTraversal(cb TraversalCallback,
	limit uint16) {

	var capacity int = (int)(queue.capacity)
	var tail_idx int = (int)(queue.tail)

	if 0 == limit {
		limit = 0xFFFF
	}
	for curr_size := queue.size; 0 != curr_size; curr_size-- {
		if 0 == limit {
			break
		} else {
			limit--
			tail_idx--

			if tail_idx < 0 {
				cb(queue.elements[capacity+tail_idx])
			} else {
				cb(queue.elements[tail_idx])
			}
		}
	}
}
