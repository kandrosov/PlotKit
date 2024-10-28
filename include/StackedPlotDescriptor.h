/*! Code to produce stacked plots.
This file is part of https://github.com/hh-italian-group/AnalysisTools. */

#pragma once

#include <THStack.h>
#include "SmartHistogram.h"
#include "DrawOptions.h"
#include "PlotDescriptor.h"
#include "RootPrintTools.h"

namespace root_ext {

class StackedPlotDescriptor : public PlotDescriptor {
public:
    using exception = ::analysis::exception;
    using Hist = SmartHistogram<TH1D>;
    using HistPtr = std::shared_ptr<Hist>;
    using HistPtrVec = std::vector<HistPtr>;
    using Graph = TGraphAsymmErrors;
    using GraphPtr = std::shared_ptr<Graph>;
    using PageOptions = draw_options::Page;
    using HistOptions = draw_options::Histogram;
    using MultiRange = Hist::MultiRange;

    StackedPlotDescriptor(const PageOptions& _page_opt, const HistOptions& _signal_opt,
                          const HistOptions& _bkg_opt, const HistOptions& _data_opt, const HistOptions& _bkg_unc_opt)
        : page_opt(_page_opt), signal_opt(_signal_opt), bkg_opt(_bkg_opt), data_opt(_data_opt),
          bkg_unc_opt(_bkg_unc_opt)
    {
    }

    void AddSignalHistogram(const Hist& original_hist, const std::string& legend_title, const Color& color,
                            double scale_factor)
    {
        auto hist = PrepareHistogram(original_hist, signal_opt, legend_title, color, signal_opt.fill_color, false,
                                    scale_factor);
        signals.push_back(hist);
    }

    void AddBackgroundHistogram(const Hist& original_hist, const std::string& legend_title, const Color& color)
    {
        auto hist = PrepareHistogram(original_hist, bkg_opt, legend_title, bkg_opt.line_color, color, false);
        backgrounds.push_back(hist);
        if(bkg_sum) {
            bkg_sum->Add(hist.get(), 1.);
        } else {
            bkg_sum = std::make_shared<Hist>(*hist);
            if(bkg_unc_opt)
                ApplyHistOptionsEx(*bkg_sum, *bkg_unc_opt);
        }
        range_tuner.Add(*bkg_sum, false, true);
    }

    void SetTotalBkg(const Hist& hist)
    {
        bkg_sum = std::make_shared<Hist>(hist);
        if(bkg_unc_opt)
            ApplyHistOptionsEx(*bkg_sum, *bkg_unc_opt);
        if(page_opt.divide_by_bin_width)
            plotting::DivideByBinWidth(*bkg_sum);
        range_tuner.Add(*bkg_sum, false, true);
    }
    void AddDataHistogram(const Hist& original_hist, const std::string& legend_title)
    {
        if(data)
            throw exception("Only one data histogram per stack is supported.");
        data = PrepareHistogram(original_hist, data_opt, legend_title, data_opt.line_color, data_opt.fill_color, true);
    }
    virtual bool HasPrintableContent() const override { return signals.size() || backgrounds.size() || data; }

    virtual void Draw(std::shared_ptr<TPad> main_pad, std::shared_ptr<TPad> ratio_pad, std::shared_ptr<TLegend> legend,
              std::vector<std::shared_ptr<TObject>>& plot_items) override
    {
        main_pad->SetLogx(page_opt.log_x);
        main_pad->SetLogy(page_opt.log_y);
        main_pad->cd();

        const bool has_ratio = ratio_pad.get() != nullptr;
        if(page_opt.y_min)
            range_tuner.y_min = *page_opt.y_min;
        auto frame_hist = range_tuner.DrawFrame(*main_pad, page_opt.log_y, page_opt.y_max_sf, page_opt.y_min_sf,
                                                page_opt.y_min_log);
        ApplyAxisSetup(*frame_hist, has_ratio);

        if(backgrounds.size()) {
            auto stack = std::make_shared<THStack>("", "");
            for (auto iter = backgrounds.rbegin(); iter != backgrounds.rend(); ++iter)
                stack->Add(iter->get());
            DrawItem(*stack, bkg_opt.draw_opt);
            plot_items.push_back(stack);
            if(bkg_unc_opt)
                DrawItem(*bkg_sum, bkg_unc_opt->draw_opt);
        }

        for(const auto& signal : signals)
            DrawItem(*signal, signal_opt.draw_opt);

        main_pad->RedrawAxis();
        if(data)
            DrawItem(*data_graph, data_opt.draw_opt);

        if(legend) {
            if(data_graph)
                legend->AddEntry(data_graph.get(), data->GetLegendTitle().c_str(), data_opt.legend_style.c_str());
            if(bkg_unc_opt)
                legend->AddEntry(bkg_sum.get(), bkg_unc_opt->legend_title.c_str(), bkg_unc_opt->legend_style.c_str());
            for(const auto& signal : signals)
                legend->AddEntry(signal.get(), signal->GetLegendTitle().c_str(), signal_opt.legend_style.c_str());
            for(const auto& background : backgrounds)
                legend->AddEntry(background.get(), background->GetLegendTitle().c_str(), bkg_opt.legend_style.c_str());
        }

        if(ratio_pad && bkg_sum && (bkg_unc_opt || data_graph)) {
            ratio_pad->cd();
            ratio_pad->SetLogx(page_opt.log_x);
            ratio_pad->SetLogy(page_opt.ratio_log_y);

            HistPtr ratio_unc_hist;
            GraphPtr ratio_graph;
            PlotRangeTuner ratio_range_tuner;
            if(bkg_unc_opt) {
                ratio_unc_hist = plotting::CreateNormalizedUncertaintyHistogram(*bkg_sum);
                plot_items.push_back(ratio_unc_hist);
                ratio_range_tuner.Add(*ratio_unc_hist, false, true);
            }
            if(data_graph) {
                ratio_graph = plotting::CreateRatioGraph(*data_graph, *bkg_sum);
                plot_items.push_back(ratio_graph);
                ratio_range_tuner.Add(*ratio_graph, false, true);
            }

            ratio_range_tuner.x_min = range_tuner.x_min;
            ratio_range_tuner.x_max = range_tuner.x_max;
            auto ratio_frame_hist = ratio_range_tuner.DrawFrame(*ratio_pad, page_opt.ratio_log_y,
                                                                page_opt.ratio_y_max_sf, page_opt.ratio_y_min_sf,
                                                                page_opt.ratio_y_min_log);
            ApplyRatioAxisSetup(*ratio_frame_hist);
            if(ratio_unc_hist)
                DrawItem(*ratio_unc_hist, bkg_unc_opt->draw_opt);
            if(ratio_graph)
                DrawItem(*ratio_graph, data_opt.draw_opt);
        }
    }

private:
    void UpdatePageOptions(const Hist& hist)
    {
        page_opt.log_x = hist.UseLogX();
        page_opt.log_y = hist.UseLogY();
        page_opt.y_max_sf = hist.MaxYDrawScaleFactor();
        page_opt.y_min_sf = hist.MinYDrawScaleFactor();
        page_opt.x_title = hist.GetXTitle();
        page_opt.y_title = hist.GetYTitle();
        page_opt.divide_by_bin_width = hist.NeedToDivideByBinWidth();
        double y_min;
        if(hist.TryGetMinY(y_min)) {
            page_opt.y_min = y_min;
            page_opt.y_min_log = y_min;
        }
    }

    template<typename Item>
    static void ApplyHistOptions(Item& item, const HistOptions& opt)
    {
        item.SetFillStyle(opt.fill_style);
        item.SetFillColor(opt.fill_color.GetColor_t());
        item.SetLineStyle(opt.line_style);
        item.SetLineWidth(opt.line_width);
        item.SetLineColor(opt.line_color.GetColor_t());
        item.SetMarkerStyle(opt.marker_style);
        item.SetMarkerSize(opt.marker_size);
        item.SetMarkerColor(opt.marker_color.GetColor_t());

    }

    static void ApplyHistOptionsEx(Hist& hist, const HistOptions& opt)
    {
        ApplyHistOptions(hist, opt);
        hist.SetLegendTitle(opt.legend_title);
    }

    HistPtr PrepareHistogram(const Hist& original_histogram, const HistOptions& opt, const std::string& legend_title,
                             const Color& line_color, const Color& fill_color, bool is_data, double scale_factor = 1.)
    {
        auto hist = std::make_shared<Hist>(original_histogram);
        auto opt_copy = opt;
        opt_copy.fill_color = fill_color;
        opt_copy.line_color = line_color;
        opt_copy.legend_title = legend_title;

        UpdatePageOptions(*hist);
        if(is_data) {
            hist->SetBinErrorOption(TH1::kPoisson);
            const auto blind_ranges = opt.blind ? hist->GetBlindRanges() : MultiRange();
            data_graph = plotting::HistogramToGraph(*hist, page_opt.divide_by_bin_width, blind_ranges);
            ApplyHistOptions(*data_graph, opt_copy);
            range_tuner.Add(*data_graph, false, true);
        }
        else {
            if(opt.apply_syst_unc) {
                hist->Scale(hist->GetPostfitScaleFactor());
                plotting::ApplyAdditionalUncertainty(*hist, hist->GetSystematicUncertainty());
            }
            if(page_opt.divide_by_bin_width)
                plotting::DivideByBinWidth(*hist);
            hist->Scale(scale_factor);
            range_tuner.Add(*hist, false, true);
        }
        ApplyHistOptionsEx(*hist, opt_copy);
        return hist;
    }

    static HistPtr CreateSumHistogram(const HistPtrVec& hists)
    {
        if(hists.empty())
            throw exception("Unable to create sum histogram.");
        auto iter = hists.begin();
        auto sum_hist = std::make_shared<Hist>(**iter++);
        for(; iter != hists.end(); ++iter)
            sum_hist->Add(iter->get(), 1.);
        return sum_hist;
    }

    void ApplyAxisSetup(TH1& frame_hist, bool has_ratio) const
    {
        frame_hist.GetYaxis()->SetTitle(page_opt.y_title.c_str());
        frame_hist.GetYaxis()->SetTitleSize(page_opt.axis_title_sizes.y());
        frame_hist.GetYaxis()->SetTitleOffset(page_opt.axis_title_offsets.y());
        frame_hist.GetYaxis()->SetLabelSize(page_opt.axis_label_sizes.y());
        frame_hist.GetYaxis()->SetLabelOffset(page_opt.axis_label_offsets.y());
        if(has_ratio) {
            frame_hist.GetXaxis()->SetTitle("");
            frame_hist.GetXaxis()->SetTitleSize(0);
            frame_hist.GetXaxis()->SetTitleOffset(0);
            frame_hist.GetXaxis()->SetLabelSize(0);
            frame_hist.GetXaxis()->SetLabelOffset(0);
        } else {
            frame_hist.GetXaxis()->SetTitle(page_opt.x_title.c_str());
            frame_hist.GetXaxis()->SetTitleSize(page_opt.axis_title_sizes.x());
            frame_hist.GetXaxis()->SetTitleOffset(page_opt.axis_title_offsets.x());
            frame_hist.GetXaxis()->SetLabelSize(page_opt.axis_label_sizes.x());
            frame_hist.GetXaxis()->SetLabelOffset(page_opt.axis_label_offsets.x());
        }
    }
    void ApplyRatioAxisSetup(TH1& frame_hist) const
    {
        const float sf = page_opt.GetRatioPadSizeSF();
        frame_hist.GetXaxis()->SetTitle(page_opt.x_title.c_str());
        frame_hist.GetXaxis()->SetTitleSize(page_opt.axis_title_sizes.x() * sf);
        frame_hist.GetXaxis()->SetTitleOffset(page_opt.axis_title_offsets.x());
        frame_hist.GetXaxis()->SetLabelSize(page_opt.axis_label_sizes.x() * sf);
        frame_hist.GetXaxis()->SetLabelOffset(page_opt.axis_label_offsets.x());
        frame_hist.GetYaxis()->SetTitle(page_opt.ratio_y_title.c_str());
        frame_hist.GetYaxis()->SetTitleSize(page_opt.ratio_y_title_size * sf);
        frame_hist.GetYaxis()->SetTitleOffset(page_opt.ratio_y_title_offset);
        frame_hist.GetYaxis()->SetLabelSize(page_opt.ratio_y_label_size * sf);
        frame_hist.GetYaxis()->SetLabelOffset(page_opt.ratio_y_label_offset);
        frame_hist.GetYaxis()->SetNdivisions(page_opt.ratio_n_div_y);
        if(page_opt.log_y)
        {
            frame_hist.GetYaxis()->SetMoreLogLabels();
            frame_hist.GetYaxis()->SetMaxDigits(2);
        }
        if(page_opt.log_x)
        {
            frame_hist.GetXaxis()->SetMoreLogLabels();
            frame_hist.GetXaxis()->SetNoExponent();
            frame_hist.GetXaxis()->SetMaxDigits(5);
        }
        if(page_opt.max_ratio > 0)
            frame_hist.GetYaxis()->SetRangeUser(std::max(0., 2 - page_opt.max_ratio), page_opt.max_ratio);
    }

    template<typename Item>
    void DrawItem(Item& item, const std::string& draw_opt) const
    {
        const auto opt = "SAME" + draw_opt;
        item.Draw(opt.c_str());
    }

private:
    HistPtrVec signals, backgrounds;
    HistPtr data, bkg_sum;
    GraphPtr data_graph;

    PageOptions page_opt;
    HistOptions signal_opt, bkg_opt, data_opt;
    std::optional<HistOptions> bkg_unc_opt;

    PlotRangeTuner range_tuner;
};

} // namespace analysis