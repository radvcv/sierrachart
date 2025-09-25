# 0DTE Options Viewer - Example Output

## Expected Badge Display

When the 0DTE Options Viewer + Net GEX plugin is active, users will see the following badges on their chart:

### Toolbar Flow Split Badge

**Position**: Top of chart, after "Largest [GEX]" indicator

**Example Displays**:

1. **Bullish Scenario** (Call OI > Put OI):
   ```
   Flow Split: 65% Call / 35% Put
   ```
   - Color: GREEN
   - Indicates more call open interest, suggesting bullish sentiment

2. **Bearish Scenario** (Put OI > Call OI):
   ```
   Flow Split: 30% Call / 70% Put
   ```
   - Color: RED
   - Indicates more put open interest, suggesting bearish sentiment

### Open Interest Chart Badges

**Position**: Main chart area, typically in upper portion

**Max Call OI Badge** (Green):
```
Max Call OI: 5,240 @ 4180
```
- Shows highest call open interest is 5,240 contracts at $4,180 strike

**Max Put OI Badge** (Red):
```
Max Put OI: 7,830 @ 4220
```
- Shows highest put open interest is 7,830 contracts at $4,220 strike

## Sample Market Scenario

**Underlying Price**: SPX @ 4200

**Options Chain Analysis**:
- Strike range analyzed: 4150 to 4250 (10 strikes each side, $5 intervals)
- Total Call OI: 45,600 contracts
- Total Put OI: 67,200 contracts
- Flow Split: 40% Call / 60% Put (RED - bearish sentiment)

**Key Levels Identified**:
- Max Call OI: 8,900 contracts @ 4195 strike (potential resistance)
- Max Put OI: 12,400 contracts @ 4205 strike (potential support)

## Interpretation Guide

### Flow Split Badge Colors:
- **GREEN**: Call dominance suggests institutional/retail preference for upside exposure
- **RED**: Put dominance suggests defensive positioning or downside hedging

### Max OI Levels:
- **Max Call OI Strike**: Often acts as resistance (sellers may defend)
- **Max Put OI Strike**: Often acts as support (put sellers defend level)

### Real-Time Updates:
- Badges refresh every 5 seconds (configurable)
- Colors change dynamically as options flow shifts
- Strike levels update as new positions are established

## Chart Layout Example

```
[Toolbar Area]
Largest [GEX]: 1.2B    Flow Split: 45% Call / 55% Put  [RED]

[Chart Area - Price Movement]
     4210 ┤
     4205 ┤  Max Put OI: 12,400 @ 4205  [RED BADGE]
     4200 ┤  ●●●●●● (current price)
     4195 ┤  Max Call OI: 8,900 @ 4195  [GREEN BADGE]
     4190 ┤
```

This visualization helps traders quickly assess:
1. Overall market sentiment (Flow Split badge)
2. Key support/resistance levels (Max OI badges)
3. Potential areas where price may gravitate or find resistance