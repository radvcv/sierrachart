#include "sierrachart.h"
#include <vector>
SCDLLName("0DTE Options Viewer + Net GEX")

/*
    0DTE Options Viewer + Net GEX Application
    
    Modificări solicitate:
    1. Adaugă un badge pe toolbar după Largest [GEX] care arată flow split pentru open interest (OI):
       - Calcul procentul Call OI vs Put OI pentru expiration-ul selectat.
       - Badge verde dacă Call OI > Put OI, roșu dacă Put OI > Call OI.
       - Textul: "Flow Split: xx% Call / yy% Put".
    2. Pe chart-ul Open Interest, badge-ul pentru Max Put OI și Max Call OI:
       - Să fie un singur badge pentru fiecare.
*/

struct OptionData {
    float Strike;
    float CallOI;
    float PutOI;
    float CallVolume;
    float PutVolume;
    float CallGEX;
    float PutGEX;
};

SCSFExport scsf_DTEOptionsViewer(SCStudyInterfaceRef sc)
{
    // Input references
    SCInputRef i_FontSize = sc.Input[0];
    SCInputRef i_HorizontalOffset = sc.Input[1]; 
    SCInputRef i_VerticalOffset = sc.Input[2];
    SCInputRef i_UpdateInterval = sc.Input[3];
    SCInputRef i_ShowFlowSplit = sc.Input[4];
    SCInputRef i_ShowMaxOI = sc.Input[5];
    SCInputRef i_StrikeInterval = sc.Input[6];
    SCInputRef i_NumStrikes = sc.Input[7];
    
    // Subgraphs for data display
    SCSubgraphRef s_FlowSplit = sc.Subgraph[0];
    SCSubgraphRef s_MaxCallOI = sc.Subgraph[1];
    SCSubgraphRef s_MaxPutOI = sc.Subgraph[2];

    // Set configuration variables
    if (sc.SetDefaults)
    {
        sc.GraphName = "0DTE Options Viewer + Net GEX";
        sc.StudyDescription = "Displays options flow split and maximum OI information";
        sc.GraphRegion = 0;
        sc.GraphShortName = "0DTE-GEX";
        sc.UpdateAlways = 1;
        
        // Input settings
        i_FontSize.Name = "Font Size";
        i_FontSize.SetInt(12);
        
        i_HorizontalOffset.Name = "Horizontal Offset (pixels)";
        i_HorizontalOffset.SetInt(20);
        
        i_VerticalOffset.Name = "Vertical Offset (pixels)";
        i_VerticalOffset.SetInt(20);
        
        i_UpdateInterval.Name = "Update Interval (seconds)";
        i_UpdateInterval.SetInt(5);
        
        i_ShowFlowSplit.Name = "Show Flow Split Badge";
        i_ShowFlowSplit.SetYesNo(1);
        
        i_ShowMaxOI.Name = "Show Max OI Badges";
        i_ShowMaxOI.SetYesNo(1);
        
        i_StrikeInterval.Name = "Strike Price Interval";
        i_StrikeInterval.SetFloat(5.0f);
        
        i_NumStrikes.Name = "Number of Strikes (each side)";
        i_NumStrikes.SetInt(10);
        
        // Subgraph settings
        s_FlowSplit.Name = "Flow Split";
        s_FlowSplit.DrawStyle = DRAWSTYLE_IGNORE;
        s_FlowSplit.PrimaryColor = COLOR_CYAN;
        
        s_MaxCallOI.Name = "Max Call OI";
        s_MaxCallOI.DrawStyle = DRAWSTYLE_IGNORE;
        s_MaxCallOI.PrimaryColor = COLOR_GREEN;
        
        s_MaxPutOI.Name = "Max Put OI";
        s_MaxPutOI.DrawStyle = DRAWSTYLE_IGNORE;
        s_MaxPutOI.PrimaryColor = COLOR_RED;
        
        return;
    }

    // Skip processing if study is hidden
    if (sc.HideStudy) return;

    // Get current options data
    std::vector<OptionData> OptionsChain;
    
    // Mock data for demonstration - in real implementation this would come from options data feed
    // For now, we'll simulate some option data
    if (sc.Index == sc.ArraySize - 1) // Only process on the last bar
    {
        // Simulate options chain data
        float UnderlyingPrice = sc.Close[sc.Index];
        
        // Generate mock options data around current price
        float StrikeInterval = i_StrikeInterval.GetFloat();
        int NumStrikes = i_NumStrikes.GetInt();
        
        for (int i = -NumStrikes; i <= NumStrikes; i++)
        {
            OptionData opt;
            opt.Strike = UnderlyingPrice + (i * StrikeInterval);
            
            // Mock OI data - simulate realistic distribution
            // Calls typically have higher OI at lower strikes (ITM)
            // Puts typically have higher OI at higher strikes (ITM)
            float distanceFromMoney = abs(opt.Strike - UnderlyingPrice);
            float baseOI = 1000.0f;
            
            if (opt.Strike < UnderlyingPrice) {
                // ITM calls, OTM puts
                opt.CallOI = baseOI + (1000.0f / (1.0f + distanceFromMoney / 10.0f));
                opt.PutOI = baseOI * 0.3f + (distanceFromMoney * 20.0f);
            } else {
                // OTM calls, ITM puts  
                opt.CallOI = baseOI * 0.3f + (distanceFromMoney * 20.0f);
                opt.PutOI = baseOI + (1000.0f / (1.0f + distanceFromMoney / 10.0f));
            }
            
            opt.CallVolume = opt.CallOI * 0.1f;
            opt.PutVolume = opt.PutOI * 0.1f;
            opt.CallGEX = opt.CallOI * opt.Strike * 0.01f;
            opt.PutGEX = opt.PutOI * opt.Strike * 0.01f;
            
            OptionsChain.push_back(opt);
        }
        
        // Calculate flow split percentages
        float TotalCallOI = 0, TotalPutOI = 0;
        float MaxCallOI = 0, MaxPutOI = 0;
        float MaxCallStrike = 0, MaxPutStrike = 0;
        
        for (const auto& opt : OptionsChain)
        {
            TotalCallOI += opt.CallOI;
            TotalPutOI += opt.PutOI;
            
            if (opt.CallOI > MaxCallOI) {
                MaxCallOI = opt.CallOI;
                MaxCallStrike = opt.Strike;
            }
            
            if (opt.PutOI > MaxPutOI) {
                MaxPutOI = opt.PutOI;
                MaxPutStrike = opt.Strike;
            }
        }
        
        float TotalOI = TotalCallOI + TotalPutOI;
        float CallOIPercent = (TotalOI > 0) ? (TotalCallOI / TotalOI) * 100.0f : 0;
        float PutOIPercent = (TotalOI > 0) ? (TotalPutOI / TotalOI) * 100.0f : 0;
        
        // Display Flow Split Badge (positioned after Largest [GEX] on toolbar)
        if (i_ShowFlowSplit.GetYesNo())
        {
            s_UseTool FlowSplitTool;
            FlowSplitTool.ChartNumber = sc.ChartNumber;
            FlowSplitTool.LineNumber = 20240101; // Unique line number for Flow Split badge
            FlowSplitTool.DrawingType = DRAWING_STATIONARY_TEXT;
            FlowSplitTool.UseRelativeVerticalValues = 1;
            FlowSplitTool.BeginValue = 98; // Top position on chart (toolbar area)
            FlowSplitTool.BeginDateTime = 10 + i_HorizontalOffset.GetInt(); // Position after Largest [GEX]
            FlowSplitTool.AddMethod = UTAM_ADD_OR_ADJUST;
            FlowSplitTool.FontSize = i_FontSize.GetInt();
            FlowSplitTool.FontBold = true;
            FlowSplitTool.TransparencyLevel = 0; // Fully opaque
            FlowSplitTool.Region = sc.GraphRegion;
            
            // Set color based on call vs put dominance
            // Badge verde dacă Call OI > Put OI, roșu dacă Put OI > Call OI
            if (CallOIPercent > PutOIPercent) {
                FlowSplitTool.Color = COLOR_GREEN;
            } else {
                FlowSplitTool.Color = COLOR_RED;
            }
            
            // Textul: "Flow Split: xx% Call / yy% Put"
            FlowSplitTool.Text.Format("Flow Split: %.0f%% Call / %.0f%% Put", 
                                     CallOIPercent, PutOIPercent);
            FlowSplitTool.AddAsUserDrawnDrawing = 1;
            sc.UseTool(FlowSplitTool);
        }
        
        // Display Max OI Badges on Open Interest Chart (single badge for each)
        if (i_ShowMaxOI.GetYesNo())
        {
            // Max Call OI Badge
            s_UseTool MaxCallTool;
            MaxCallTool.ChartNumber = sc.ChartNumber;
            MaxCallTool.LineNumber = 20240102; // Unique line number for Max Call OI
            MaxCallTool.DrawingType = DRAWING_STATIONARY_TEXT;
            MaxCallTool.UseRelativeVerticalValues = 1;
            MaxCallTool.BeginValue = 90; // Position on chart for Open Interest display
            MaxCallTool.BeginDateTime = 20 + i_HorizontalOffset.GetInt();
            MaxCallTool.AddMethod = UTAM_ADD_OR_ADJUST;
            MaxCallTool.FontSize = i_FontSize.GetInt();
            MaxCallTool.FontBold = true;
            MaxCallTool.TransparencyLevel = 0;
            MaxCallTool.Region = sc.GraphRegion;
            MaxCallTool.Color = COLOR_GREEN;
            MaxCallTool.Text.Format("Max Call OI: %.0f @ %.0f", MaxCallOI, MaxCallStrike);
            MaxCallTool.AddAsUserDrawnDrawing = 1;
            sc.UseTool(MaxCallTool);
            
            // Max Put OI Badge  
            s_UseTool MaxPutTool;
            MaxPutTool.ChartNumber = sc.ChartNumber;
            MaxPutTool.LineNumber = 20240103; // Unique line number for Max Put OI
            MaxPutTool.DrawingType = DRAWING_STATIONARY_TEXT;
            MaxPutTool.UseRelativeVerticalValues = 1;
            MaxPutTool.BeginValue = 85; // Slightly below Max Call OI badge
            MaxPutTool.BeginDateTime = 20 + i_HorizontalOffset.GetInt();
            MaxPutTool.AddMethod = UTAM_ADD_OR_ADJUST;
            MaxPutTool.FontSize = i_FontSize.GetInt();
            MaxPutTool.FontBold = true;
            MaxPutTool.TransparencyLevel = 0;
            MaxPutTool.Region = sc.GraphRegion;
            MaxPutTool.Color = COLOR_RED;
            MaxPutTool.Text.Format("Max Put OI: %.0f @ %.0f", MaxPutOI, MaxPutStrike);
            MaxPutTool.AddAsUserDrawnDrawing = 1;
            sc.UseTool(MaxPutTool);
        }
        
        // Store values in subgraphs for reference
        s_FlowSplit[sc.Index] = CallOIPercent;
        s_MaxCallOI[sc.Index] = MaxCallOI;
        s_MaxPutOI[sc.Index] = MaxPutOI;
    }
}