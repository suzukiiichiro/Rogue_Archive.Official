package main

type stats struct {
	Story         []string
	Killed        int
	KilledMons    map[monsterKind]int
	Moves         int
	Hits          int
	Misses        int
	ReceivedHits  int
	Dodges        int
	Blocks        int
	Drinks        int
	Evocations    int
	Throws        int
	TimesLucky    int
	Damage        int
	DExplPerc     [MaxDepth + 1]int
	DSleepingPerc [MaxDepth + 1]int
	DKilledPerc   [MaxDepth + 1]int
	DLayout       [MaxDepth + 1]string
	Burns         int
	Digs          int
	Rest          int
	RestInterrupt int
	Turns         int
	TWounded      int
	TMWounded     int
	TMonsLOS      int
	UsedRod       [NumRods]int
	//UsedPotion    [MaxDepth + 1]int
	//UsedProj      [MaxDepth + 1]int
}

func (g *game) TurnStats() {
	g.Stats.Turns++
	g.DepthPlayerTurn++
	if g.Player.HP < g.Player.HPMax() {
		g.Stats.TWounded++
	}
	if g.MonsterInLOS() != nil {
		g.Stats.TMonsLOS++
		if g.Player.HP < g.Player.HPMax() {
			g.Stats.TMWounded++
		}
	}
}

func (g *game) LevelStats() {
	free := 0
	exp := 0
	for _, c := range g.Dungeon.Cells {
		if c.T != FreeCell {
			continue
		}
		free++
		if c.Explored {
			exp++
		}
	}
	g.Stats.DExplPerc[g.Depth] = exp * 100 / free
	//g.Stats.DBurns[g.Depth] = g.Stats.CurBurns // XXX to avoid little dump info leak
	nmons := len(g.Monsters)
	kmons := 0
	smons := 0
	for _, mons := range g.Monsters {
		if !mons.Exists() {
			kmons++
			continue
		}
		if mons.State == Resting {
			smons++
		}
	}
	g.Stats.DSleepingPerc[g.Depth] = smons * 100 / nmons
	g.Stats.DKilledPerc[g.Depth] = kmons * 100 / nmons
}
