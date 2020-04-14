package main

import (
	"bytes"
	"container/heap"
	"fmt"
)

type Dijkstrer interface {
	Neighbors(position) []position
	Cost(position, position) int
}

func (g *game) drawDijkstra(nm nodeMap) string {
	// TODO: this function could be removed
	b := &bytes.Buffer{}
	for y := 0; y < DungeonHeight; y++ {
		for x := 0; x < DungeonWidth; x++ {
			pos := position{x, y}
			n, ok := nm[pos]
			if ok {
				if pos == g.Player.Pos {
					fmt.Fprintf(b, "%d@", n.Cost)
				} else {
					c := g.Dungeon.Cell(pos)
					if c.T == WallCell {
						fmt.Fprintf(b, "%d#", n.Cost)
					} else {
						fmt.Fprintf(b, "%d ", n.Cost)
					}
				}
			} else {
				fmt.Fprintf(b, "# ")
			}
		}
		fmt.Fprintf(b, "\n")
	}
	return b.String()
}

func Dijkstra(dij Dijkstrer, sources []position, maxCost int) nodeMap {
	nodeCache = nodeCache[:0]
	nm := nodeMap{}
	nq := &priorityQueue{}
	heap.Init(nq)
	for _, f := range sources {
		n := nm.get(f)
		n.Open = true
		heap.Push(nq, n)
	}
	for {
		if nq.Len() == 0 {
			return nm
		}
		current := heap.Pop(nq).(*node)
		current.Open = false
		current.Closed = true

		for _, neighbor := range dij.Neighbors(current.Pos) {
			cost := current.Cost + dij.Cost(current.Pos, neighbor)
			neighborNode := nm.get(neighbor)
			if cost < neighborNode.Cost {
				if neighborNode.Open {
					heap.Remove(nq, neighborNode.Index)
				}
				neighborNode.Open = false
				neighborNode.Closed = false
			}
			if !neighborNode.Open && !neighborNode.Closed {
				neighborNode.Cost = cost
				if cost < maxCost {
					neighborNode.Open = true
					neighborNode.Rank = cost
					heap.Push(nq, neighborNode)
				}
			}
		}
	}
}

const unreachable = 9999

func (g *game) AutoExploreDijkstra(dij Dijkstrer, sources []int) {
	d := g.Dungeon
	dmap := DijkstraMapCache[:]
	var visited [DungeonNCells]bool
	var queue [DungeonNCells]int
	var qstart, qend int
	for i := 0; i < DungeonNCells; i++ {
		dmap[i] = unreachable
	}
	for _, s := range sources {
		dmap[s] = 0
		queue[qend] = s
		qend++
		visited[s] = true
	}
	for qstart < qend {
		cidx := queue[qstart]
		qstart++
		cpos := idxtopos(cidx)
		for _, npos := range dij.Neighbors(cpos) {
			nidx := npos.idx()
			if !npos.valid() || d.Cells[nidx].T == WallCell {
				continue
			}
			if !visited[nidx] {
				queue[qend] = nidx
				qend++
				visited[nidx] = true
				dmap[nidx] = 1 + dmap[cidx]
			}
		}
	}
}
