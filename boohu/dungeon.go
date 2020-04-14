// many ideas here from articles found at http://www.roguebasin.com/

package main

import (
	"sort"
)

type dungeon struct {
	Gen   dungen
	Cells []cell
}

type cell struct {
	T        terrain
	Explored bool
}

type terrain int

const (
	WallCell terrain = iota
	FreeCell
)

type dungen int

const (
	GenCaveMap dungen = iota
	GenRoomMap
	GenCellularAutomataCaveMap
	GenCaveMapTree
	GenRuinsMap
	GenBSPMap
)

func (dg dungen) Use(g *game) {
	switch dg {
	case GenCaveMap:
		g.GenCaveMap(DungeonHeight, DungeonWidth)
	case GenRoomMap:
		g.GenRoomMap(DungeonHeight, DungeonWidth)
	case GenCellularAutomataCaveMap:
		g.GenCellularAutomataCaveMap(DungeonHeight, DungeonWidth)
	case GenCaveMapTree:
		g.GenCaveMapTree(DungeonHeight, DungeonWidth)
	case GenRuinsMap:
		g.GenRuinsMap(DungeonHeight, DungeonWidth)
	case GenBSPMap:
		g.GenBSPMap(DungeonHeight, DungeonWidth)
	}
	g.Dungeon.Gen = dg
	g.Stats.DLayout[g.Depth] = dg.String()
}

func (dg dungen) String() (text string) {
	switch dg {
	case GenCaveMap:
		text = "OC"
	case GenRoomMap:
		text = "BR"
	case GenCellularAutomataCaveMap:
		text = "EC"
	case GenCaveMapTree:
		text = "TC"
	case GenRuinsMap:
		text = "RR"
	case GenBSPMap:
		text = "DT"
	}
	return text
}

func (dg dungen) Description() (text string) {
	switch dg {
	case GenCaveMap:
		text = "open cave"
	case GenRoomMap:
		text = "big rooms"
	case GenCellularAutomataCaveMap:
		text = "eight cave"
	case GenCaveMapTree:
		text = "tree-like cave"
	case GenRuinsMap:
		text = "ruined rooms"
	case GenBSPMap:
		text = "deserted town"
	}
	return text
}

type room struct {
	pos position
	w   int
	h   int
}

func (d *dungeon) Cell(pos position) cell {
	return d.Cells[pos.idx()]
}

func (d *dungeon) Border(pos position) bool {
	return pos.X == DungeonWidth-1 || pos.Y == DungeonHeight-1 || pos.X == 0 || pos.Y == 0
}

func (d *dungeon) SetCell(pos position, t terrain) {
	d.Cells[pos.idx()].T = t
}

func (d *dungeon) SetExplored(pos position) {
	d.Cells[pos.idx()].Explored = true
}

func roomDistance(r1, r2 room) int {
	return Abs(r1.pos.X-r2.pos.X) + Abs(r1.pos.Y-r2.pos.Y)
}

func nearRoom(rooms []room, r room) room {
	closest := rooms[0]
	d := roomDistance(r, closest)
	for _, nextRoom := range rooms {
		nd := roomDistance(r, nextRoom)
		if nd < d {
			n := RandInt(10)
			if n > 3 {
				d = nd
				closest = nextRoom
			}
		}
	}
	return closest
}

func nearestRoom(rooms []room, r room) room {
	closest := rooms[0]
	d := roomDistance(r, closest)
	for _, nextRoom := range rooms {
		nd := roomDistance(r, nextRoom)
		if nd < d {
			n := RandInt(10)
			if n > 0 {
				d = nd
				closest = nextRoom
			}
		}
	}
	return closest
}

func intersectsRoom(rooms []room, r room) bool {
	for _, rr := range rooms {
		if (r.pos.X+r.w-1 >= rr.pos.X && rr.pos.X+rr.w-1 >= r.pos.X) &&
			(r.pos.Y+r.h-1 >= rr.pos.Y && rr.pos.Y+rr.h-1 >= r.pos.Y) {
			return true
		}
	}
	return false
}

func (d *dungeon) connectRooms(r1, r2 room) {
	x := r1.pos.X
	if x < r2.pos.X {
		x += r1.w - 1
	}
	y := r1.pos.Y
	if y < r2.pos.Y {
		y += r1.h - 1
	}
	d.SetCell(position{x, y}, FreeCell)
	count := 0
	for {
		count++
		if count > 1000 {
			panic("ConnectRooms")
		}
		if x < r2.pos.X {
			x++
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if x > r2.pos.X {
			x--
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if y < r2.pos.Y {
			y++
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if y > r2.pos.Y {
			y--
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		break
	}
	d.SetCell(r2.pos, FreeCell)
}

func (d *dungeon) connectRoomsDiagonally(r1, r2 room) {
	x := r1.pos.X
	if x < r2.pos.X {
		x += r1.w - 1
	}
	y := r1.pos.Y
	if y < r2.pos.Y {
		y += r1.h - 1
	}
	d.SetCell(position{x, y}, FreeCell)
	count := 0
	for {
		count++
		if count > 1000 {
			panic("ConnectRooms")
		}
		if x < r2.pos.X && y < r2.pos.Y {
			x++
			d.SetCell(position{x, y}, FreeCell)
			y++
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if x > r2.pos.X && y < r2.pos.Y {
			x--
			d.SetCell(position{x, y}, FreeCell)
			y++
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if x > r2.pos.X && y > r2.pos.Y {
			x--
			d.SetCell(position{x, y}, FreeCell)
			y--
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if x < r2.pos.X && y > r2.pos.Y {
			x++
			d.SetCell(position{x, y}, FreeCell)
			y--
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if x < r2.pos.X {
			x++
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if x > r2.pos.X {
			x--
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if y < r2.pos.Y {
			y++
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		if y > r2.pos.Y {
			y--
			d.SetCell(position{x, y}, FreeCell)
			continue
		}
		break
	}
	d.SetCell(r2.pos, FreeCell)
}

func (d *dungeon) Area(area []position, pos position, radius int) []position {
	area = area[:0]
	for x := pos.X - radius; x <= pos.X+radius; x++ {
		for y := pos.Y - radius; y <= pos.Y+radius; y++ {
			pos := position{x, y}
			if pos.valid() {
				area = append(area, pos)
			}
		}
	}
	return area
}

func (d *dungeon) ConnectRoomsShortestPath(r1, r2 room) {
	var r1pos, r2pos position
	r1pos.X = r1.pos.X + RandInt(r1.w)
	if r1pos.X < r2.pos.X {
		r1pos.X = r1.pos.X + r1.w - 1
	}
	r1pos.Y = r1.pos.Y + RandInt(r1.h)
	if r1pos.Y < r2.pos.Y {
		r1pos.Y = r1.pos.Y + r1.h - 1
	}
	r2pos.X = r2.pos.X + RandInt(r2.w)
	if r2pos.X < r1.pos.X {
		r2pos.X = r2.pos.X + r2.w - 1
	}
	r2pos.Y = r2.pos.Y + RandInt(r2.h)
	if r2pos.Y < r1.pos.Y {
		r2pos.Y = r2.pos.Y + r2.h - 1
	}
	mp := &dungeonPath{dungeon: d}
	path, _, _ := AstarPath(mp, r1pos, r2pos)
	for _, pos := range path {
		d.SetCell(pos, FreeCell)
	}
}

func (d *dungeon) ConnectIsolatedRoom(doorpos position) {
	for i := 0; i < 200; i++ {
		pos := d.FreeCell()
		dp := &dungeonPath{dungeon: d, wcost: unreachable}
		path, _, _ := AstarPath(dp, pos, doorpos)
		wall := false
		for _, pos := range path {
			if d.Cell(pos).T == WallCell {
				wall = true
				break
			}
		}
		if !wall {
			continue
		}
		for _, pos := range path {
			d.SetCell(pos, FreeCell)
		}
		break
	}
}

func (d *dungeon) DigRoom(r room) {
	for i := r.pos.X; i < r.pos.X+r.w; i++ {
		for j := r.pos.Y; j < r.pos.Y+r.h; j++ {
			rpos := position{i, j}
			if rpos.valid() {
				d.SetCell(rpos, FreeCell)
			}
		}
	}
}

func (d *dungeon) PutCols(r room) {
	for i := r.pos.X + 1; i < r.pos.X+r.w-1; i += 2 {
		for j := r.pos.Y + 1; j < r.pos.Y+r.h-1; j += 2 {
			rpos := position{i, j}
			if rpos.valid() {
				d.SetCell(rpos, WallCell)
			}
		}
	}
}

func (d *dungeon) PutDiagCols(r room) {
	n := RandInt(2)
	for i := r.pos.X + 1; i < r.pos.X+r.w-1; i++ {
		m := n
		for j := r.pos.Y + 1; j < r.pos.Y+r.h-1; j++ {
			rpos := position{i, j}
			if rpos.valid() && m%2 == 0 {
				d.SetCell(rpos, WallCell)
			}
			m++
		}
		n++
	}
}

func (d *dungeon) IsAreaFree(pos position, h, w int) bool {
	for i := pos.X; i < pos.X+w; i++ {
		for j := pos.Y; j < pos.Y+h; j++ {
			rpos := position{i, j}
			if !rpos.valid() || d.Cell(rpos).T != FreeCell {
				return false
			}
		}
	}
	return true
}

func (d *dungeon) RoomDigCanditate(pos position, h, w int) (ret bool) {
	for i := pos.X; i < pos.X+w; i++ {
		for j := pos.Y; j < pos.Y+h; j++ {
			rpos := position{i, j}
			if !rpos.valid() {
				return false
			}
			if d.Cell(rpos).T == FreeCell {
				ret = true
			}
		}
	}
	return ret
}

func (d *dungeon) IsolatedRoomDigCanditate(pos position, h, w int) (ret bool) {
	for i := pos.X; i < pos.X+w; i++ {
		for j := pos.Y; j < pos.Y+h; j++ {
			rpos := position{i, j}
			if !rpos.valid() {
				return false
			}
			if d.Cell(rpos).T == FreeCell {
				return false
			}
		}
	}
	return true
}

func (d *dungeon) DigArea(pos position, h, w int) {
	for i := pos.X; i < pos.X+w; i++ {
		for j := pos.Y; j < pos.Y+h; j++ {
			rpos := position{i, j}
			if !rpos.valid() {
				continue
			}
			d.SetCell(rpos, FreeCell)
		}
	}
}

func (d *dungeon) BlockArea(pos position, h, w int) {
	// not used now
	for i := pos.X; i < pos.X+w; i++ {
		for j := pos.Y; j < pos.Y+h; j++ {
			rpos := position{i, j}
			if !rpos.valid() {
				continue
			}
			d.SetCell(rpos, WallCell)
		}
	}
}

func (d *dungeon) BuildRoom(pos position, w, h int, outside bool) map[position]bool {
	spos := position{pos.X - 1, pos.Y - 1}
	if outside && !d.IsAreaFree(spos, h+2, w+2) {
		return nil
	}
	for i := pos.X; i < pos.X+w; i++ {
		d.SetCell(position{i, pos.Y}, WallCell)
		d.SetCell(position{i, pos.Y + h - 1}, WallCell)
	}
	for i := pos.Y; i < pos.Y+h; i++ {
		d.SetCell(position{pos.X, i}, WallCell)
		d.SetCell(position{pos.X + w - 1, i}, WallCell)
	}
	if RandInt(2) == 0 || !outside {
		n := RandInt(2)
		for x := pos.X + 1; x < pos.X+w-1; x++ {
			m := n
			for y := pos.Y + 1; y < pos.Y+h-1; y++ {
				if m%2 == 0 {
					d.SetCell(position{x, y}, WallCell)
				}
				m++
			}
			n++
		}
	} else {
		n := RandInt(2)
		m := RandInt(2)
		//if n == 0 && m == 0 {
		//// round room
		//d.SetCell(pos, FreeCell)
		//d.SetCell(position{pos.X, pos.Y + h - 1}, FreeCell)
		//d.SetCell(position{pos.X + w - 1, pos.Y}, FreeCell)
		//d.SetCell(position{pos.X + w - 1, pos.Y + h - 1}, FreeCell)
		//}
		for x := pos.X + 1 + m; x < pos.X+w-1; x += 2 {
			for y := pos.Y + 1 + n; y < pos.Y+h-1; y += 2 {
				d.SetCell(position{x, y}, WallCell)
			}
		}

	}
	area := make([]position, 9)
	if outside {
		for _, p := range [4]position{pos, {pos.X, pos.Y + h - 1}, {pos.X + w - 1, pos.Y}, {pos.X + w - 1, pos.Y + h - 1}} {
			if d.WallAreaCount(area, p, 1) == 4 {
				d.SetCell(p, FreeCell)
			}
		}
	}
	doorsc := [4]position{
		position{pos.X + w/2, pos.Y},
		position{pos.X + w/2, pos.Y + h - 1},
		position{pos.X, pos.Y + h/2},
		position{pos.X + w - 1, pos.Y + h/2},
	}
	doors := make(map[position]bool)
	for i := 0; i < 3+RandInt(2); i++ {
		dpos := doorsc[RandInt(4)]
		doors[dpos] = true
		d.SetCell(dpos, FreeCell)
	}
	return doors
}

func (d *dungeon) BuildSomeRoom(w, h int) map[position]bool {
	for i := 0; i < 200; i++ {
		pos := d.FreeCell()
		doors := d.BuildRoom(pos, w, h, true)
		if doors != nil {
			return doors
		}
	}
	return nil
}

func (d *dungeon) DigSomeRoom(w, h int) map[position]bool {
	for i := 0; i < 200; i++ {
		pos := d.FreeCell()
		dpos := position{pos.X - 1, pos.Y - 1}
		if !d.RoomDigCanditate(dpos, h+2, w+2) {
			continue
		}
		d.DigArea(dpos, h+2, w+2)
		doors := d.BuildRoom(pos, w, h, true)
		if doors != nil {
			return doors
		}
	}
	return nil
}

func (d *dungeon) DigIsolatedRoom(w, h int) map[position]bool {
	i := RandInt(DungeonNCells)
	for j := 0; j < DungeonNCells; j++ {
		i = (i + 1) % DungeonNCells
		pos := idxtopos(i)
		if d.Cells[i].T == FreeCell {
			continue
		}
		dpos := position{pos.X - 1, pos.Y - 1}
		if !d.IsolatedRoomDigCanditate(dpos, h+2, w+2) {
			continue
		}
		d.DigArea(pos, h, w)
		doors := d.BuildRoom(pos, w, h, false)
		if doors != nil {
			return doors
		}
	}
	return nil
}

func (d *dungeon) ResizeRoom(r room) room {
	if DungeonWidth-r.pos.X < r.w {
		r.w = DungeonWidth - r.pos.X
	}
	if DungeonHeight-r.pos.Y < r.h {
		r.h = DungeonHeight - r.pos.Y
	}
	return r
}

func (g *game) GenRuinsMap(h, w int) {
	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	rooms := []room{}
	for i := 0; i < 43; i++ {
		var ro room
		count := 100
		for count > 0 {
			count--
			ro = room{
				pos: position{RandInt(w - 1), RandInt(h - 1)},
				w:   3 + RandInt(5),
				h:   2 + RandInt(3)}
			ro = d.ResizeRoom(ro)
			if !intersectsRoom(rooms, ro) {
				break
			}
		}

		d.DigRoom(ro)
		if RandInt(60) == 0 {
			if RandInt(2) == 0 {
				d.PutCols(ro)
			} else {
				d.PutDiagCols(ro)
			}
		}
		if len(rooms) > 0 {
			r := RandInt(100)
			if r > 75 {
				d.connectRooms(nearRoom(rooms, ro), ro)
			} else if r > 25 {
				d.ConnectRoomsShortestPath(nearRoom(rooms, ro), ro)
			} else {
				d.connectRoomsDiagonally(nearRoom(rooms, ro), ro)
			}
		}
		rooms = append(rooms, ro)
	}
	doors := d.DigSomeRooms(5)
	g.Dungeon = d
	g.Fungus = make(map[position]vegetation)
	g.DigFungus(1 + RandInt(2))
	g.PutDoors(30)
	g.PutDoorsList(doors, 20)
}

func (g *game) DigFungus(n int) {
	d := g.Dungeon
	count := 0
	fungus := g.Foliage(DungeonHeight, DungeonWidth)
loop:
	for i := 0; i < 100; i++ {
		if count > 100 {
			break loop
		}
		if n <= 0 {
			break
		}
		pos := d.FreeCell()
		if _, ok := fungus[pos]; ok {
			continue
		}
		conn, count := d.Connected(pos, func(npos position) bool {
			_, ok := fungus[npos]
			//return ok && d.IsFreeCell(npos)
			return ok
		})
		if count < 3 {
			continue
		}
		if len(conn) > 150 {
			continue
		}
		for cpos := range conn {
			d.SetCell(cpos, FreeCell)
			g.Fungus[cpos] = foliage
			count++
		}
		n--
	}
}

type roomSlice []room

func (rs roomSlice) Len() int      { return len(rs) }
func (rs roomSlice) Swap(i, j int) { rs[i], rs[j] = rs[j], rs[i] }
func (rs roomSlice) Less(i, j int) bool {
	return rs[i].pos.Y < rs[j].pos.Y || rs[i].pos.Y == rs[j].pos.Y && rs[i].pos.X < rs[j].pos.X
}

func (g *game) GenRoomMap(h, w int) {
	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	rooms := []room{}
	cols := 0
	for i := 0; i < 35; i++ {
		var ro room
		count := 100
		for count > 0 {
			count--
			ro = room{
				pos: position{RandInt(w - 1), RandInt(h - 1)},
				w:   5 + RandInt(4),
				h:   3 + RandInt(3)}
			ro = d.ResizeRoom(ro)
			if !intersectsRoom(rooms, ro) {
				break
			}
		}

		d.DigRoom(ro)
		if RandInt(10+15*cols) == 0 {
			if RandInt(2) == 0 {
				d.PutCols(ro)
			} else {
				d.PutDiagCols(ro)
			}
			cols++
		}
		rooms = append(rooms, ro)
	}
	sort.Sort(roomSlice(rooms))
	for i, ro := range rooms {
		if i == 0 {
			continue
		}
		r := RandInt(100)
		if r > 50 {
			d.connectRooms(nearestRoom(rooms[:i], ro), ro)
		} else if r > 25 {
			d.ConnectRoomsShortestPath(nearRoom(rooms[:i], ro), ro)
		} else {
			d.connectRoomsDiagonally(nearestRoom(rooms[:i], ro), ro)
		}
	}
	g.Dungeon = d
	doors := d.DigSomeRooms(5)
	g.PutDoors(90)
	g.PutDoorsList(doors, 10)
}

func (g *game) PutDoorsList(doors map[position]bool, threshold int) {
	for pos := range doors {
		if g.DoorCandidate(pos) && RandInt(100) > threshold {
			g.Doors[pos] = true
			if _, ok := g.Fungus[pos]; ok {
				delete(g.Fungus, pos)
			}
		}
	}
}

func (d *dungeon) FreeCell() position {
	count := 0
	for {
		count++
		if count > 1000 {
			panic("FreeCell")
		}
		x := RandInt(DungeonWidth)
		y := RandInt(DungeonHeight)
		pos := position{x, y}
		c := d.Cell(pos)
		if c.T == FreeCell {
			return pos
		}
	}
}

func (d *dungeon) WallCell() position {
	count := 0
	for {
		count++
		if count > 1000 {
			panic("WallCell")
		}
		x := RandInt(DungeonWidth)
		y := RandInt(DungeonHeight)
		pos := position{x, y}
		c := d.Cell(pos)
		if c.T == WallCell {
			return pos
		}
	}
}

func (g *game) GenCaveMap(h, w int) {
	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	pos := position{40, 10}
	max := 21 * 42
	d.SetCell(pos, FreeCell)
	cells := 1
	notValid := 0
	lastValid := pos
	diag := RandInt(4) == 0
	for cells < max {
		npos := pos.RandomNeighbor(diag)
		if !pos.valid() && npos.valid() && d.Cell(npos).T == WallCell {
			pos = lastValid
			continue
		}
		pos = npos
		if pos.valid() {
			if d.Cell(pos).T != FreeCell {
				d.SetCell(pos, FreeCell)
				cells++
			}
			lastValid = pos
		} else {
			notValid++
		}
		if notValid > 200 {
			notValid = 0
			pos = lastValid
		}
	}
	cells = 1
	max = DungeonHeight * 1
	digs := 0
	i := 0
	block := make([]position, 0, 64)
loop:
	for cells < max {
		i++
		if digs > 3 {
			break
		}
		if i > 1000 {
			break
		}
		diag = RandInt(2) == 0
		block = d.DigBlock(block, diag)
		if len(block) == 0 {
			continue loop
		}
		if len(block) < 4 || len(block) > 10 {
			continue loop
		}
		for _, pos := range block {
			d.SetCell(pos, FreeCell)
			cells++
		}
		digs++
	}
	doors := make(map[position]bool)
	rooms := 0
	if RandInt(4) > 0 {
		w, h := GenCaveRoomSize()
		rooms++
		for pos := range d.BuildSomeRoom(w, h) {
			doors[pos] = true
		}
		if RandInt(7) == 0 {
			rooms++
			w, h := GenCaveRoomSize()
			for pos := range d.BuildSomeRoom(w, h) {
				doors[pos] = true
			}

		}
	}
	if RandInt(1+rooms) == 0 {
		w, h := GenLittleRoomSize()
		i := 0
		for pos := range d.DigIsolatedRoom(w, h) {
			doors[pos] = true
			if i == 0 {
				d.ConnectIsolatedRoom(pos)
			}
			i++
		}

	}
	g.Dungeon = d
	g.Fungus = g.Foliage(DungeonHeight, DungeonWidth)
	g.PutDoors(5)
	for pos := range doors {
		if g.DoorCandidate(pos) && RandInt(100) > 20 {
			g.Doors[pos] = true
			if _, ok := g.Fungus[pos]; ok {
				delete(g.Fungus, pos)
			}
		}
	}
}

func GenCaveRoomSize() (int, int) {
	return 7 + 2*RandInt(2), 5 + 2*RandInt(2)
}

func GenLittleRoomSize() (int, int) {
	return 7, 5
}

func (d *dungeon) HasFreeNeighbor(pos position) bool {
	neighbors := pos.ValidNeighbors()
	for _, pos := range neighbors {
		if d.Cell(pos).T == FreeCell {
			return true
		}
	}
	return false
}

func (g *game) HasFreeExploredNeighbor(pos position) bool {
	d := g.Dungeon
	neighbors := pos.ValidNeighbors()
	for _, pos := range neighbors {
		c := d.Cell(pos)
		if c.T == FreeCell && c.Explored && !g.WrongWall[pos] {
			return true
		}
	}
	return false
}

func (d *dungeon) DigBlock(block []position, diag bool) []position {
	pos := d.WallCell()
	block = block[:0]
	for {
		block = append(block, pos)
		if d.HasFreeNeighbor(pos) {
			break
		}
		pos = pos.RandomNeighbor(diag)
		if !pos.valid() {
			block = block[:0]
			pos = d.WallCell()
			continue
		}
		if !pos.valid() {
			return nil
		}
	}
	return block
}

func (g *game) GenCaveMapTree(h, w int) {
	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	center := position{40, 10}
	d.SetCell(center, FreeCell)
	d.SetCell(center.E(), FreeCell)
	d.SetCell(center.NE(), FreeCell)
	d.SetCell(center.S(), FreeCell)
	d.SetCell(center.SE(), FreeCell)
	d.SetCell(center.N(), FreeCell)
	d.SetCell(center.NW(), FreeCell)
	d.SetCell(center.W(), FreeCell)
	d.SetCell(center.SW(), FreeCell)
	max := 21 * 23
	cells := 1
	diag := RandInt(2) == 0
	block := make([]position, 0, 64)
loop:
	for cells < max {
		block = d.DigBlock(block, diag)
		if len(block) == 0 {
			continue loop
		}
		for _, pos := range block {
			if d.Cell(pos).T != FreeCell {
				d.SetCell(pos, FreeCell)
				cells++
			}
		}
	}

	doors := d.DigSomeRooms(5)
	g.Dungeon = d
	g.Fungus = make(map[position]vegetation)
	g.DigFungus(1 + RandInt(2))
	g.PutDoors(5)
	g.PutDoorsList(doors, 20)
}

func (d *dungeon) DigSomeRooms(chances int) map[position]bool {
	doors := make(map[position]bool)
	if RandInt(chances) > 0 {
		w, h := GenCaveRoomSize()
		for pos := range d.DigSomeRoom(w, h) {
			doors[pos] = true
		}
		if RandInt(3) == 0 {
			w, h := GenCaveRoomSize()
			for pos := range d.DigSomeRoom(w, h) {
				doors[pos] = true
			}
		}
	}
	return doors
}

func (d *dungeon) WallAreaCount(area []position, pos position, radius int) int {
	area = d.Area(area, pos, radius)
	count := 0
	for _, npos := range area {
		if d.Cell(npos).T == WallCell {
			count++
		}
	}
	switch radius {
	case 1:
		count += 9 - len(area)
	case 2:
		count += 25 - len(area)
	}
	return count
}

func (d *dungeon) Connected(pos position, nf func(position) bool) (map[position]bool, int) {
	conn := map[position]bool{}
	stack := []position{pos}
	count := 0
	conn[pos] = true
	nb := make([]position, 0, 8)
	for len(stack) > 0 {
		pos = stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		count++
		nb = pos.Neighbors(nb, nf)
		for _, npos := range nb {
			if !conn[npos] {
				conn[npos] = true
				stack = append(stack, npos)
			}
		}
	}
	return conn, count
}

func (d *dungeon) connex() bool {
	pos := d.FreeCell()
	conn, _ := d.Connected(pos, d.IsFreeCell)
	for i, c := range d.Cells {
		if c.T == FreeCell && !conn[idxtopos(i)] {
			return false
		}
	}
	return true
}

func (g *game) RunCellularAutomataCave(h, w int) bool {
	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	for i := range d.Cells {
		r := RandInt(100)
		pos := idxtopos(i)
		if r >= 45 {
			d.SetCell(pos, FreeCell)
		} else {
			d.SetCell(pos, WallCell)
		}
	}
	bufm := &dungeon{}
	bufm.Cells = make([]cell, h*w)
	area := make([]position, 0, 25)
	for i := 0; i < 5; i++ {
		for j := range bufm.Cells {
			pos := idxtopos(j)
			c1 := d.WallAreaCount(area, pos, 1)
			if c1 >= 5 {
				bufm.SetCell(pos, WallCell)
			} else {
				bufm.SetCell(pos, FreeCell)
			}
			if i == 3 {
				c2 := d.WallAreaCount(area, pos, 2)
				if c2 <= 2 {
					bufm.SetCell(pos, WallCell)
				}
			}
		}
		copy(d.Cells, bufm.Cells)
	}
	var conn map[position]bool
	var count int
	var winner position
	for i := 0; i < 15; i++ {
		pos := d.FreeCell()
		if conn[pos] {
			continue
		}
		var ncount int
		conn, ncount = d.Connected(pos, d.IsFreeCell)
		if ncount > count {
			count = ncount
			winner = pos
		}
		if count >= 37*DungeonHeight*DungeonWidth/100 {
			break
		}
	}
	conn, count = d.Connected(winner, d.IsFreeCell)
	if count <= 37*DungeonHeight*DungeonWidth/100 {
		return false
	}
	for i, c := range d.Cells {
		pos := idxtopos(i)
		if c.T == FreeCell && !conn[pos] {
			d.SetCell(pos, WallCell)
		}
	}
	digs := 0
	max := DungeonHeight / 2
	cells := 1
	i := 0
	block := make([]position, 0, 64)
loop:
	for cells < max {
		i++
		if digs > 3 {
			break
		}
		if i > 1000 {
			break
		}
		diag := RandInt(2) == 0
		block = d.DigBlock(block, diag)
		if len(block) == 0 {
			continue loop
		}
		if len(block) < 4 || len(block) > 10 {
			continue loop
		}
		for _, pos := range block {
			d.SetCell(pos, FreeCell)
			cells++
		}
		digs++
	}
	doors := make(map[position]bool)
	if RandInt(5) > 0 {
		w, h := GenLittleRoomSize()
		i := 0
		for pos := range d.DigIsolatedRoom(w, h) {
			doors[pos] = true
			if i == 0 {
				d.ConnectIsolatedRoom(pos)
			}
			i++
		}
		if RandInt(4) == 0 {
			w, h := GenCaveRoomSize()
			i := 0
			for pos := range d.DigIsolatedRoom(w, h) {
				doors[pos] = true
				if i == 0 {
					d.ConnectIsolatedRoom(pos)
				}
				i++
			}
		}
	}
	g.Dungeon = d
	g.PutDoors(10)
	for pos := range doors {
		if g.DoorCandidate(pos) && RandInt(100) > 20 {
			g.Doors[pos] = true
			if _, ok := g.Fungus[pos]; ok {
				delete(g.Fungus, pos)
			}
		}
	}
	return true
}

func (g *game) GenCellularAutomataCaveMap(h, w int) {
	count := 0
	for {
		count++
		if count > 100 {
			panic("genCellularAutomataCaveMap")
		}
		if g.RunCellularAutomataCave(h, w) {
			break
		}
	}
	g.Fungus = g.Foliage(DungeonHeight, DungeonWidth)
}

func (d *dungeon) SimpleRoom(r room) map[position]bool {
	for i := r.pos.X; i < r.pos.X+r.w; i++ {
		d.SetCell(position{i, r.pos.Y}, WallCell)
		d.SetCell(position{i, r.pos.Y + r.h - 1}, WallCell)
	}
	for i := r.pos.Y; i < r.pos.Y+r.h; i++ {
		d.SetCell(position{r.pos.X, i}, WallCell)
		d.SetCell(position{r.pos.X + r.w - 1, i}, WallCell)
	}
	doorsc := [4]position{
		position{r.pos.X + r.w/2, r.pos.Y},
		position{r.pos.X + r.w/2, r.pos.Y + r.h - 1},
		position{r.pos.X, r.pos.Y + r.h/2},
		position{r.pos.X + r.w - 1, r.pos.Y + r.h/2},
	}
	doors := make(map[position]bool)
	for i := 0; i < 3+RandInt(2); i++ {
		dpos := doorsc[RandInt(4)]
		doors[dpos] = true
		d.SetCell(dpos, FreeCell)
	}
	return doors
}

func (g *game) ExtendEdgeRoom(r room, doors map[position]bool) room {
	if g.Dungeon.Cell(r.pos).T != WallCell {
		return r
	}
	extend := false
	if r.pos.X+r.w+1 == DungeonWidth {
		for i := r.pos.Y + 1; i < r.pos.Y+r.h-1; i++ {
			g.Dungeon.SetCell(position{DungeonWidth - 2, i}, FreeCell)
			g.Dungeon.SetCell(position{DungeonWidth - 1, i}, WallCell)
		}
		g.Dungeon.SetCell(position{DungeonWidth - 1, r.pos.Y}, WallCell)
		g.Dungeon.SetCell(position{DungeonWidth - 1, r.pos.Y + r.h - 1}, WallCell)
		g.Dungeon.SetCell(position{DungeonWidth - 2, r.pos.Y + 1}, WallCell)
		g.Dungeon.SetCell(position{DungeonWidth - 2, r.pos.Y + r.h - 2}, WallCell)
		r.w++
		extend = true
	}
	if r.pos.X == 1 {
		for i := r.pos.Y + 1; i < r.pos.Y+r.h-1; i++ {
			g.Dungeon.SetCell(position{1, i}, FreeCell)
			g.Dungeon.SetCell(position{0, i}, WallCell)
		}
		g.Dungeon.SetCell(position{0, r.pos.Y}, WallCell)
		g.Dungeon.SetCell(position{0, r.pos.Y + r.h - 1}, WallCell)
		g.Dungeon.SetCell(position{1, r.pos.Y + 1}, WallCell)
		g.Dungeon.SetCell(position{1, r.pos.Y + r.h - 2}, WallCell)
		r.w++
		r.pos.X--
		extend = true
	}
	if r.pos.Y+r.h+1 == DungeonHeight {
		for i := r.pos.X + 1; i < r.pos.X+r.w-1; i++ {
			g.Dungeon.SetCell(position{i, DungeonHeight - 2}, FreeCell)
			g.Dungeon.SetCell(position{i, DungeonHeight - 1}, WallCell)
		}
		g.Dungeon.SetCell(position{r.pos.X, DungeonHeight - 1}, WallCell)
		g.Dungeon.SetCell(position{r.pos.X + r.w - 1, DungeonHeight - 1}, WallCell)
		g.Dungeon.SetCell(position{r.pos.X + 1, DungeonHeight - 2}, WallCell)
		g.Dungeon.SetCell(position{r.pos.X + r.w - 2, DungeonHeight - 2}, WallCell)
		r.h++
		extend = true
	}
	if r.pos.Y == 1 {
		for i := r.pos.X + 1; i < r.pos.X+r.w-1; i++ {
			g.Dungeon.SetCell(position{i, 1}, FreeCell)
			g.Dungeon.SetCell(position{i, 0}, WallCell)
		}
		g.Dungeon.SetCell(position{r.pos.X, 0}, WallCell)
		g.Dungeon.SetCell(position{r.pos.X + r.w - 1, 0}, WallCell)
		g.Dungeon.SetCell(position{r.pos.X + 1, 1}, WallCell)
		g.Dungeon.SetCell(position{r.pos.X + r.w - 2, 1}, WallCell)
		r.h++
		r.pos.Y--
		extend = true
	}
	if !extend {
		return r
	}
	for pos := range doors {
		if pos.X == 1 || pos.X == DungeonWidth-2 || pos.Y == 1 || pos.Y == DungeonHeight-2 {
			delete(g.Doors, pos)
			continue
		}
	}
	doorsc := [4]position{
		position{r.pos.X + r.w/2, r.pos.Y},
		position{r.pos.X + r.w/2, r.pos.Y + r.h - 1},
		position{r.pos.X, r.pos.Y + r.h/2},
		position{r.pos.X + r.w - 1, r.pos.Y + r.h/2},
	}
	ndoorsc := []position{}
	ndoors := 0
	for _, pos := range doorsc {
		if pos.X == 0 || pos.X == DungeonWidth-1 || pos.Y == 0 || pos.Y == DungeonHeight-1 {
			continue
		}
		if g.Doors[pos] {
			ndoors++
		}
		ndoorsc = append(ndoorsc, pos)
	}
	for i := 0; i < 1+RandInt(2-ndoors); i++ {
		dpos := ndoorsc[RandInt(len(ndoorsc))]
		g.Doors[dpos] = true
		g.Dungeon.SetCell(dpos, FreeCell)
	}
	return r
}

func (g *game) DivideRoomVertically(r room) {
	if g.Dungeon.Cell(r.pos).T != WallCell {
		return
	}
	if r.w <= 6 {
		return
	}
	if r.h < 5 {
		return
	}
	dx := 2 + RandInt(r.w/2-2)
	if RandInt(2) == 0 {
		dx = r.w - 3 - RandInt(r.w/2-3)
	}
	if dx == 2 && r.pos.X == 0 {
		return
	}
	if dx == r.w-3 && r.pos.X+r.w == DungeonWidth {
		return
	}
	free := true
loop:
	for i := r.pos.Y + 1; i < r.pos.Y+r.h-1; i++ {
		for j := dx - 1; j <= dx+1; j++ {
			if g.Dungeon.Cell(position{r.pos.X + j, i}).T == WallCell {
				free = false
				break loop
			}
		}
	}
	if !free {
		return
	}
	for i := r.pos.Y + 1; i < r.pos.Y+r.h-1; i++ {
		g.Dungeon.SetCell(position{r.pos.X + dx, i}, WallCell)
	}
	doorpos := position{r.pos.X + dx, r.pos.Y + r.h/2}
	g.Doors[doorpos] = true
	g.Dungeon.SetCell(doorpos, FreeCell)
}

func (g *game) DivideRoomHorizontally(r room) {
	if g.Dungeon.Cell(r.pos).T != WallCell {
		return
	}
	if r.h <= 6 {
		return
	}
	if r.w < 5 {
		return
	}
	dy := 2 + RandInt(r.h/2-2)
	if RandInt(2) == 0 {
		dy = r.h - 3 - RandInt(r.h/2-3)
	}
	if dy == 2 && r.pos.Y == 0 {
		return
	}
	if dy == r.h-3 && r.pos.Y+r.h == DungeonHeight {
		return
	}
	free := true
loop:
	for i := r.pos.X + 1; i < r.pos.X+r.w-1; i++ {
		for j := dy - 1; j <= dy+1; j++ {
			if g.Dungeon.Cell(position{i, r.pos.Y + j}).T == WallCell {
				free = false
				break loop
			}
		}
	}
	if !free {
		return
	}
	for i := r.pos.X + 1; i < r.pos.X+r.w-1; i++ {
		g.Dungeon.SetCell(position{i, r.pos.Y + dy}, WallCell)
	}
	doorpos := position{r.pos.X + r.w/2, r.pos.Y + dy}
	g.Doors[doorpos] = true
	g.Dungeon.SetCell(doorpos, FreeCell)
}

func (g *game) GenBSPMap(h, w int) {
	rooms := []room{}
	crooms := []room{{pos: position{1, 1}, w: DungeonWidth - 2, h: DungeonHeight - 2}}
	big := 0
	for len(crooms) > 0 {
		r := crooms[0]
		crooms = crooms[1:]
		if r.h <= 8 && r.w <= 12 {
			switch RandInt(6) {
			case 0:
				if r.h >= 6 {
					r.h--
					if RandInt(2) == 0 {
						r.pos.Y++
					}
				}
			case 1:
				if r.w >= 8 {
					r.w--
					if RandInt(2) == 0 {
						r.pos.X++
					}
				}
			}
			if r.h > 2 && r.w > 2 {
				rooms = append(rooms, r)
			}
			continue
		}
		if RandInt(2+big) == 0 && (r.h <= 12 && r.w <= 20) {
			big++
			switch RandInt(4) {
			case 0:
				r.h--
				if RandInt(2) == 0 {
					r.pos.Y++
				}
			case 1:
				r.w--
				if RandInt(2) == 0 {
					r.pos.X++
				}
			}
			if r.h > 2 && r.w > 2 {
				rooms = append(rooms, r)
			}
			continue
		}
		horizontal := false
		if r.h > 8 && r.w > 10 && r.w < 40 && RandInt(4) == 0 {
			horizontal = true
		} else if r.h > 8 && r.w <= 10+RandInt(5) {
			horizontal = true
		}
		if horizontal {
			h := r.h/2 - r.h/4 + RandInt(1+r.h/2)
			if h <= 3 {
				h++
			}
			if r.h-h-1 <= 3 {
				h--
			}
			crooms = append(crooms, room{r.pos, r.w, h}, room{position{r.pos.X, r.pos.Y + 1 + h}, r.w, r.h - h - 1})
		} else {
			w := r.w/2 - r.w/4 + RandInt(1+r.w/2)
			if w <= 3 {
				w++
			}
			if r.w-w-1 <= 3 {
				w--
			}
			crooms = append(crooms, room{r.pos, w, r.h}, room{position{r.pos.X + 1 + w, r.pos.Y}, r.w - w - 1, r.h})
		}
	}

	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	for i := 0; i < DungeonNCells; i++ {
		d.SetCell(idxtopos(i), FreeCell)
	}
	g.Dungeon = d
	g.Doors = map[position]bool{}
	special := 0
	empty := 0
	for i, r := range rooms {
		var doors map[position]bool
		if RandInt(2+special/3) == 0 && r.w%2 == 1 && r.h%2 == 1 && r.w >= 5 && r.h >= 5 {
			doors = d.BuildRoom(r.pos, r.w, r.h, true)
			special++
		} else if empty > 0 || RandInt(20) > 0 {
			doors = d.SimpleRoom(r)
			if RandInt(2) == 0 && r.w >= 7 && r.h >= 7 {
				rn := r
				rn.pos.X++
				rn.pos.Y++
				rn.h--
				rn.h--
				rn.w--
				rn.w--
				if RandInt(2) == 0 {
					d.PutCols(rn)
				} else {
					d.PutDiagCols(rn)
				}
			} else if RandInt(1+special/2) == 0 && r.w >= 11 && r.h >= 9 {
				sx := (r.w - 11) / 2
				sy := (r.h - 9) / 2
				doors = d.BuildRoom(position{r.pos.X + 2 + sx, r.pos.Y + 2 + sy}, 7, 5, true)
				special++
			}
		} else {
			empty++
		}
		for pos := range doors {
			if g.DoorCandidate(pos) && RandInt(100) > 10 {
				g.Doors[pos] = true
			}
		}
		if RandInt(2) == 0 {
			r = g.ExtendEdgeRoom(r, doors)
			rooms[i] = r
		}
		if RandInt(5) > 0 {
			if RandInt(2) == 0 {
				g.DivideRoomVertically(r)
			} else {
				g.DivideRoomHorizontally(r)
			}
		}
	}
	g.Fungus = make(map[position]vegetation)
	g.DigFungus(RandInt(3))
	for i := 0; i <= RandInt(2); i++ {
		r := rooms[RandInt(len(rooms))]
		for x := r.pos.X + 1; x < r.pos.X+r.w-1; x++ {
			for y := r.pos.Y + 1; y < r.pos.Y+r.h-1; y++ {
				g.Fungus[position{x, y}] = foliage
			}
		}
	}
}

type vegetation int

const (
	foliage vegetation = iota
)

func (g *game) Foliage(h, w int) map[position]vegetation {
	// use same structure as for the dungeon
	// walls will become foliage
	d := &dungeon{}
	d.Cells = make([]cell, h*w)
	for i := range d.Cells {
		r := RandInt(100)
		pos := idxtopos(i)
		if r >= 43 {
			d.SetCell(pos, WallCell)
		} else {
			d.SetCell(pos, FreeCell)
		}
	}
	area := make([]position, 0, 25)
	for i := 0; i < 6; i++ {
		bufm := &dungeon{}
		bufm.Cells = make([]cell, h*w)
		copy(bufm.Cells, d.Cells)
		for j := range bufm.Cells {
			pos := idxtopos(j)
			c1 := d.WallAreaCount(area, pos, 1)
			if i < 4 {
				if c1 <= 4 {
					bufm.SetCell(pos, FreeCell)
				} else {
					bufm.SetCell(pos, WallCell)
				}
			}
			if i == 4 {
				if c1 > 6 {
					bufm.SetCell(pos, WallCell)
				}
			}
			if i == 5 {
				c2 := d.WallAreaCount(area, pos, 2)
				if c2 < 5 && c1 <= 2 {
					bufm.SetCell(pos, FreeCell)
				}
			}
		}
		d.Cells = bufm.Cells
	}
	fungus := make(map[position]vegetation)
	for i, c := range d.Cells {
		if _, ok := g.Doors[idxtopos(i)]; !ok && c.T == FreeCell {
			fungus[idxtopos(i)] = foliage
		}
	}
	return fungus
}

func (g *game) DoorCandidate(pos position) bool {
	d := g.Dungeon
	if !pos.valid() || d.Cell(pos).T != FreeCell {
		return false
	}
	return pos.W().valid() && pos.E().valid() &&
		d.Cell(pos.W()).T == FreeCell && d.Cell(pos.E()).T == FreeCell &&
		!g.Doors[pos.W()] && !g.Doors[pos.E()] &&
		(!pos.N().valid() || d.Cell(pos.N()).T == WallCell) &&
		(!pos.S().valid() || d.Cell(pos.S()).T == WallCell) &&
		((pos.NW().valid() && d.Cell(pos.NW()).T == FreeCell) ||
			(pos.SW().valid() && d.Cell(pos.SW()).T == FreeCell) ||
			(pos.NE().valid() && d.Cell(pos.NE()).T == FreeCell) ||
			(pos.SE().valid() && d.Cell(pos.SE()).T == FreeCell)) ||
		pos.N().valid() && pos.S().valid() &&
			d.Cell(pos.N()).T == FreeCell && d.Cell(pos.S()).T == FreeCell &&
			!g.Doors[pos.N()] && !g.Doors[pos.S()] &&
			(!pos.E().valid() || d.Cell(pos.E()).T == WallCell) &&
			(!pos.W().valid() || d.Cell(pos.W()).T == WallCell) &&
			((pos.NW().valid() && d.Cell(pos.NW()).T == FreeCell) ||
				(pos.SW().valid() && d.Cell(pos.SW()).T == FreeCell) ||
				(pos.NE().valid() && d.Cell(pos.NE()).T == FreeCell) ||
				(pos.SE().valid() && d.Cell(pos.SE()).T == FreeCell))
}

func (g *game) PutDoors(percentage int) {
	g.Doors = map[position]bool{}
	for i := range g.Dungeon.Cells {
		pos := idxtopos(i)
		if g.DoorCandidate(pos) && RandInt(100) < percentage {
			g.Doors[pos] = true
			if _, ok := g.Fungus[pos]; ok {
				delete(g.Fungus, pos)
			}
		}
	}
}
