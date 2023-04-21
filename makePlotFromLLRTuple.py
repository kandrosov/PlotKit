import ROOT
ROOT.gStyle.SetOptStat(0)
if __name__ == "__main__":
    #import argparse

    import os
    path = "/eos/user/l/lportale/hhbbtautau/skims/SKIMS_UL18/"
    ggSig = ["GluGluToBulkGravitonToHHTo2B2Tau_M-1000_", "GluGluToBulkGravitonToHHTo2B2Tau_M-1250_", "GluGluToBulkGravitonToHHTo2B2Tau_M-1500_", "GluGluToBulkGravitonToHHTo2B2Tau_M-1750_", "GluGluToBulkGravitonToHHTo2B2Tau_M-2000_", "GluGluToBulkGravitonToHHTo2B2Tau_M-250_", "GluGluToBulkGravitonToHHTo2B2Tau_M-2500_", "GluGluToBulkGravitonToHHTo2B2Tau_M-260_", "GluGluToBulkGravitonToHHTo2B2Tau_M-270_", "GluGluToBulkGravitonToHHTo2B2Tau_M-280_", "GluGluToBulkGravitonToHHTo2B2Tau_M-300_", "GluGluToBulkGravitonToHHTo2B2Tau_M-3000_", "GluGluToBulkGravitonToHHTo2B2Tau_M-320_", "GluGluToBulkGravitonToHHTo2B2Tau_M-350_", "GluGluToBulkGravitonToHHTo2B2Tau_M-400_", "GluGluToBulkGravitonToHHTo2B2Tau_M-450_", "GluGluToBulkGravitonToHHTo2B2Tau_M-500_", "GluGluToBulkGravitonToHHTo2B2Tau_M-550_", "GluGluToBulkGravitonToHHTo2B2Tau_M-600_", "GluGluToBulkGravitonToHHTo2B2Tau_M-650_", "GluGluToBulkGravitonToHHTo2B2Tau_M-700_", "GluGluToBulkGravitonToHHTo2B2Tau_M-750_", "GluGluToBulkGravitonToHHTo2B2Tau_M-800_", "GluGluToBulkGravitonToHHTo2B2Tau_M-850_", "GluGluToBulkGravitonToHHTo2B2Tau_M-900_", "GluGluToRadionToHHTo2B2Tau_M-1000_", "GluGluToRadionToHHTo2B2Tau_M-1250_", "GluGluToRadionToHHTo2B2Tau_M-1500_", "GluGluToRadionToHHTo2B2Tau_M-1750_", "GluGluToRadionToHHTo2B2Tau_M-2000_", "GluGluToRadionToHHTo2B2Tau_M-250_", "GluGluToRadionToHHTo2B2Tau_M-2500_", "GluGluToRadionToHHTo2B2Tau_M-260_", "GluGluToRadionToHHTo2B2Tau_M-270_", "GluGluToRadionToHHTo2B2Tau_M-280_", "GluGluToRadionToHHTo2B2Tau_M-300_", "GluGluToRadionToHHTo2B2Tau_M-3000_", "GluGluToRadionToHHTo2B2Tau_M-320_", "GluGluToRadionToHHTo2B2Tau_M-350_", "GluGluToRadionToHHTo2B2Tau_M-400_", "GluGluToRadionToHHTo2B2Tau_M-450_", "GluGluToRadionToHHTo2B2Tau_M-500_", "GluGluToRadionToHHTo2B2Tau_M-550_", "GluGluToRadionToHHTo2B2Tau_M-600_", "GluGluToRadionToHHTo2B2Tau_M-650_", "GluGluToRadionToHHTo2B2Tau_M-700_", "GluGluToRadionToHHTo2B2Tau_M-750_", "GluGluToRadionToHHTo2B2Tau_M-800_", "GluGluToRadionToHHTo2B2Tau_M-850_", "GluGluToRadionToHHTo2B2Tau_M-900_"]
    VBFSig = ["VBFToBulkGravitonToHHTo2B2Tau_M-1000_", "VBFToBulkGravitonToHHTo2B2Tau_M-1250_", "VBFToBulkGravitonToHHTo2B2Tau_M-1500_", "VBFToBulkGravitonToHHTo2B2Tau_M-1750_", "VBFToBulkGravitonToHHTo2B2Tau_M-2000_", "VBFToBulkGravitonToHHTo2B2Tau_M-250_", "VBFToBulkGravitonToHHTo2B2Tau_M-2500_", "VBFToBulkGravitonToHHTo2B2Tau_M-260_", "VBFToBulkGravitonToHHTo2B2Tau_M-270_", "VBFToBulkGravitonToHHTo2B2Tau_M-280_", "VBFToBulkGravitonToHHTo2B2Tau_M-300_", "VBFToBulkGravitonToHHTo2B2Tau_M-3000_", "VBFToBulkGravitonToHHTo2B2Tau_M-320_", "VBFToBulkGravitonToHHTo2B2Tau_M-350_", "VBFToBulkGravitonToHHTo2B2Tau_M-400_", "VBFToBulkGravitonToHHTo2B2Tau_M-450_", "VBFToBulkGravitonToHHTo2B2Tau_M-500_", "VBFToBulkGravitonToHHTo2B2Tau_M-550_", "VBFToBulkGravitonToHHTo2B2Tau_M-600_", "VBFToBulkGravitonToHHTo2B2Tau_M-650_", "VBFToBulkGravitonToHHTo2B2Tau_M-700_", "VBFToBulkGravitonToHHTo2B2Tau_M-750_", "VBFToBulkGravitonToHHTo2B2Tau_M-800_", "VBFToBulkGravitonToHHTo2B2Tau_M-850_", "VBFToBulkGravitonToHHTo2B2Tau_M-900_", "VBFToRadionToHHTo2B2Tau_M-1000_", "VBFToRadionToHHTo2B2Tau_M-1250_", "VBFToRadionToHHTo2B2Tau_M-1500_", "VBFToRadionToHHTo2B2Tau_M-1750_", "VBFToRadionToHHTo2B2Tau_M-2000_", "VBFToRadionToHHTo2B2Tau_M-250_", "VBFToRadionToHHTo2B2Tau_M-2500_", "VBFToRadionToHHTo2B2Tau_M-260_", "VBFToRadionToHHTo2B2Tau_M-270_", "VBFToRadionToHHTo2B2Tau_M-280_", "VBFToRadionToHHTo2B2Tau_M-300_", "VBFToRadionToHHTo2B2Tau_M-3000_", "VBFToRadionToHHTo2B2Tau_M-320_", "VBFToRadionToHHTo2B2Tau_M-350_", "VBFToRadionToHHTo2B2Tau_M-400_", "VBFToRadionToHHTo2B2Tau_M-450_", "VBFToRadionToHHTo2B2Tau_M-500_", "VBFToRadionToHHTo2B2Tau_M-550_", "VBFToRadionToHHTo2B2Tau_M-600_", "VBFToRadionToHHTo2B2Tau_M-650_", "VBFToRadionToHHTo2B2Tau_M-700_", "VBFToRadionToHHTo2B2Tau_M-750_", "VBFToRadionToHHTo2B2Tau_M-800_", "VBFToRadionToHHTo2B2Tau_M-850_", "VBFToRadionToHHTo2B2Tau_M-900_"]
    all_signals = ggSig + VBFSig
    DY = ["DYJetsToLL_0J", "DYJetsToLL_LHEFilterPtZ-0To50", "DYJetsToLL_LHEFilterPtZ-400To650", "DYJetsToLL_M-50_TuneCP5_13TeV-amc", "DYJetsToLL_1J", "DYJetsToLL_LHEFilterPtZ-100To250", "DYJetsToLL_LHEFilterPtZ-50To100","DYJetsToLL_2J", "DYJetsToLL_LHEFilterPtZ-250To400","DYJetsToLL_LHEFilterPtZ-650ToInf"]
    TT = ["TTTo2L2Nu", "TTToHadronic", "TTToSemiLeptonic"]

    #parser = argparse.ArgumentParser()]
    #parser.add_argument('--period', required=False, type=str, default = 'Run2_2018')
    #parser.add_argument('--version', required=False, type=str, default = 'v2_deepTau_v2p1')
    #parser.add_argument('--vars', required=False, type=str, default = 'tau1_pt')
    #parser.add_argument('--mass', required=False, type=int, default=500)
    #parser.add_argument('--new-weights', required=False, type=bool, default=False)
    #args = parser.parse_args()
    #observables = ["genjet2_e/"]
# bjet1_JER/ bjet1_e, bjet1_pt,
    dataframes = {}
    samples = {"sig":all_signals, "TT":TT, "DY":DY}
    for sample in samples.keys():
        rootFiles = [path+ s + "/" + "*.root" for s in samples[sample] ]
        df = ROOT.RDataFrame("HTauTauTree", rootFiles)
        for jet_idx in [1,2]:
            df = df.Define(f"bjet{jet_idx}_res", f"bjet{jet_idx}_JER/bjet{jet_idx}_e").Filter(f"bjet{jet_idx}_gen_matched").Filter(f"bjet{jet_idx}_res<0.4")
            histo = df.Histo1D(f"bjet{jet_idx}_res")

            c1 = ROOT.TCanvas()
            histo.Draw()
            c1.SaveAs(f"/afs/cern.ch/work/v/vdamante/hhbbTauTauRes/prod/Framework/output/plots/sync/bjet{jet_idx}_res_{sample}.pdf", "pdf")
            model = ROOT.RDF.TH2DModel(f"bjet{jet_idx}_res_vs_pt", f"bjet{jet_idx}_res_vs_pt", 40, 0., 400., 50, 0., 0.5)

            histo2D = df.Histo2D(model, f"bjet{jet_idx}_pt",f"bjet{jet_idx}_res")
            histo2D.GetXaxis().SetTitle(f"bjet{jet_idx}_pt")
            histo2D.GetYaxis().SetTitle(f"bjet{jet_idx}_res")
            #histo2D = df.Histo2D(f"bjet{jet_idx}_pt",f"bjet{jet_idx}_res")
            c2 = ROOT.TCanvas()
            histo2D.Draw("COLZ")
            c2.SaveAs(f"/afs/cern.ch/work/v/vdamante/hhbbTauTauRes/prod/Framework/output/plots/sync/bjet{jet_idx}_resVSPt_{sample}.pdf", "pdf")

            model = ROOT.RDF.TH2DModel(f"bjet{jet_idx}_genPt_vs_pt", f"bjet{jet_idx}_genPt_vs_pt", 40, 0., 400., 40, 0., 400)
            histo2D_2 = df.Histo2D(model, f"genjet{jet_idx}_pt",f"bjet{jet_idx}_pt")
            histo2D_2.GetXaxis().SetTitle(f"genjet{jet_idx}_pt")
            histo2D_2.GetYaxis().SetTitle(f"bjet{jet_idx}_pt")

            c2_2 = ROOT.TCanvas()
            histo2D_2.Draw("COLZ")
            c2_2.SaveAs(f"/afs/cern.ch/work/v/vdamante/hhbbTauTauRes/prod/Framework/output/plots/sync/bjet{jet_idx}_genPtVSPt_{sample}.pdf", "pdf")




