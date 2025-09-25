# 0DTE Options Viewer + Net GEX

## Overview

This Sierra Chart plugin provides real-time options flow analysis for 0DTE (Zero Days To Expiration) options, displaying key metrics directly on your chart.

## Features

### 1. Flow Split Badge
- **Location**: Positioned on the toolbar after the "Largest [GEX]" indicator
- **Content**: Shows the percentage split between Call OI and Put OI
- **Format**: "Flow Split: XX% Call / YY% Put"
- **Color Coding**:
  - **Green**: When Call OI > Put OI (bullish sentiment)
  - **Red**: When Put OI > Call OI (bearish sentiment)

### 2. Maximum Open Interest Badges
- **Location**: Displayed on the Open Interest chart area
- **Content**: Two separate badges showing:
  - Max Call OI: Maximum call open interest and its strike price
  - Max Put OI: Maximum put open interest and its strike price
- **Color Coding**:
  - Max Call OI: Green
  - Max Put OI: Red

## Configuration Options

The plugin provides several customizable inputs:

- **Font Size**: Adjusts the size of all badge text (default: 12)
- **Horizontal Offset**: Pixel offset for horizontal positioning (default: 20)
- **Vertical Offset**: Pixel offset for vertical positioning (default: 20)
- **Update Interval**: How frequently to refresh data in seconds (default: 5)
- **Show Flow Split Badge**: Enable/disable the flow split display (default: Yes)
- **Show Max OI Badges**: Enable/disable the max OI displays (default: Yes)
- **Strike Price Interval**: Dollar interval between strike prices (default: $5.00)
- **Number of Strikes**: How many strikes to analyze on each side of current price (default: 10)

## Installation

1. Compile the `dte_options_viewer.cpp` file using the Sierra Chart development environment
2. Copy the resulting DLL to your Sierra Chart Data folder
3. Add the study to your chart through Analysis > Studies > Add Custom Study

## Usage

1. Apply the study to your options chart
2. Configure the input parameters as desired
3. The badges will automatically update based on the configured update interval
4. Monitor the Flow Split badge color for sentiment changes:
   - Green indicates call dominance (potentially bullish)
   - Red indicates put dominance (potentially bearish)

## Technical Details

### Data Processing
- Analyzes options chain data around the current underlying price
- Calculates total Call OI and Put OI across all configured strikes
- Identifies strikes with maximum open interest for calls and puts
- Updates displays in real-time based on market data

### Badge Positioning
- Flow Split badge uses `DRAWING_STATIONARY_TEXT` positioned relative to chart coordinates
- Max OI badges are positioned on the chart region for Open Interest display
- All badges use unique line numbers to prevent conflicts

### Mock Data for Testing
The current implementation includes realistic mock data generation for testing purposes. In a production environment, this would be replaced with actual options market data feeds.

## Requirements

- Sierra Chart with ACSIL (Advanced Custom Study Interface and Language) support
- Access to options market data feed
- Visual Studio or compatible C++ compiler for building

## Notes

- This plugin is designed specifically for 0DTE options analysis
- The Flow Split calculation provides a quick sentiment indicator based on open interest distribution
- Max OI badges help identify key support/resistance levels based on options positioning
- All badges are user-drawn drawings that persist across chart sessions