/*
 * FatJetInfoFiller.cc
 *
 *  Created on: May 24, 2017
 *      Author: hqu
 */

#include "DeepNTuples/Ntupler/interface/FatJetInfoFiller.h"
#include <string>
#include <algorithm>


#include <iostream>
#include <typeinfo>
#include <Math/Vector4D.h>

namespace deepntuples {

void FatJetInfoFiller::readConfig(const edm::ParameterSet& iConfig, edm::ConsumesCollector&& cc) {
  genParticlesToken_ = cc.consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"));
  fjTagInfoName = iConfig.getParameter<std::string>("fjTagInfoName");
  useReclusteredJets_ = iConfig.getParameter<bool>("useReclusteredJets");
  isQCDSample_ = iConfig.getUntrackedParameter<bool>("isQCDSample", false);
  sampleType_ = iConfig.getParameter<std::string>("sampleType");
  min_LHE_HT_ = int(iConfig.getParameter<double>("minLheHT"));
  sample_use_pythia_ = iConfig.getParameter<bool>("isPythia");
  sample_use_herwig_ = iConfig.getParameter<bool>("isHerwig");
  sample_use_madgraph_ = iConfig.getParameter<bool>("isMadGraph");
  isTrainSample_ = iConfig.getUntrackedParameter<bool>("isTrainSample", false);
  fjName = iConfig.getParameter<std::string>("jetType") + std::to_string(int(10*jetR_));
}

void FatJetInfoFiller::readEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  iEvent.getByToken(genParticlesToken_, genParticlesHandle);
}

void FatJetInfoFiller::book() {
  // truth labels
  data.add<int>("fj_label", 0);
  data.add<int>("fj_isTT", 0);
  data.add<int>("fj_isTop", 0);
  data.add<int>("fj_isW", 0);
  data.add<int>("fj_isZ", 0);
  data.add<int>("fj_isH", 0);
  data.add<int>("fj_isISR", 0);
  data.add<int>("fj_isQCD", 0);
  data.add<int>("fj_isNonRes", 0);

  // Non-resonant final states from ttbar, top, W, or Z
  data.add<int>("label_NR_LF",  0);
  data.add<int>("label_NR_b",   0);
  data.add<int>("label_NR_bb",  0);
  data.add<int>("label_NR_lep", 0);
  data.add<int>("label_NR_ll",  0);

  // "TT" includes all AK8 jets from ttbar events
  // "l" includes electrons or muons from W or W --> tau decays
  // "q" includes charm quarks and W --> tau --> hadron decays
  data.add<int>("label_TT_ISR", 0);
  data.add<int>("label_TT_q", 0);
  data.add<int>("label_TT_b", 0);
  data.add<int>("label_TT_bq", 0);
  data.add<int>("label_TT_qq", 0);
  data.add<int>("label_TT_bqq", 0);
  data.add<int>("label_TT_l", 0);
  data.add<int>("label_TT_bl", 0);
  data.add<int>("label_TT_q_q", 0);
  data.add<int>("label_TT_b_q", 0);
  data.add<int>("label_TT_bq_q", 0);
  data.add<int>("label_TT_qq_q", 0);
  data.add<int>("label_TT_bqq_q", 0);
  data.add<int>("label_TT_l_q", 0);
  data.add<int>("label_TT_bl_q", 0);
  data.add<int>("label_TT_b_b", 0);
  data.add<int>("label_TT_bq_b", 0);
  data.add<int>("label_TT_qq_b", 0);
  data.add<int>("label_TT_bqq_b", 0);
  data.add<int>("label_TT_l_b", 0);
  data.add<int>("label_TT_bl_b", 0);
  data.add<int>("label_TT_bq_bq", 0);
  data.add<int>("label_TT_qq_bq", 0);
  data.add<int>("label_TT_bqq_bq", 0);
  data.add<int>("label_TT_l_bq", 0);
  data.add<int>("label_TT_bl_bq", 0);
  data.add<int>("label_TT_qq_qq", 0);
  data.add<int>("label_TT_bqq_qq", 0);
  data.add<int>("label_TT_l_qq", 0);
  data.add<int>("label_TT_bl_qq", 0);
  data.add<int>("label_TT_bqq_bqq", 0);
  data.add<int>("label_TT_l_bqq", 0);
  data.add<int>("label_TT_bl_bqq", 0);
  data.add<int>("label_TT_l_l", 0);
  data.add<int>("label_TT_bl_l", 0);
  data.add<int>("label_TT_bl_bl", 0);

  data.add<int>("label_Top_bcq", 0);
  data.add<int>("label_Top_bqq", 0);
  data.add<int>("label_Top_bc",  0);
  data.add<int>("label_Top_bq",  0);
  data.add<int>("label_Top_bqt",  0); // q, tau --> had
  data.add<int>("label_Top_bele",0);
  data.add<int>("label_Top_bmu", 0);
  data.add<int>("label_Top_btauH",0); // tau --> had
  data.add<int>("label_Top_btauL",0); // tau --> lep
  data.add<int>("label_Top_bl",  0); // e, mu
  data.add<int>("label_Top_blt", 0); // e, mu, tau --> lep
  data.add<int>("label_Top_b", 0);
  data.add<int>("label_Top_ISR", 0);

  data.add<int>("label_W_cq",  0);
  data.add<int>("label_W_qq",  0);
  data.add<int>("label_W_c",   0);
  data.add<int>("label_W_q",   0);
  data.add<int>("label_W_ISR", 0);

  data.add<int>("label_Z_bb",  0);
  data.add<int>("label_Z_cc",  0);
  data.add<int>("label_Z_qq",  0);
  data.add<int>("label_Z_b",   0);
  data.add<int>("label_Z_c",   0);
  data.add<int>("label_Z_q",   0);
  data.add<int>("label_Z_ISR", 0);

  data.add<int>("label_H_bb",    0);
  data.add<int>("label_H_cc",    0);
  data.add<int>("label_H_qq",    0);
  data.add<int>("label_H_qqqq",  0);
  data.add<int>("label_H_tautau",0);
  data.add<int>("label_H_aa_bbbb",  0);
  data.add<int>("label_H_aa_other", 0);

  data.add<int>("label_QCD_bb",  0);
  data.add<int>("label_QCD_cc",  0);
  data.add<int>("label_QCD_b",   0);
  data.add<int>("label_QCD_c",   0);
  data.add<int>("label_QCD_others", 0);
  data.add<int>("label_QCD_BGen", 0);
  data.add<int>("label_QCD_bEnr", 0);
  data.add<int>("label_QCD_Incl", 0);

  data.add<int>("sample_isQCD", 0);
  data.add<int>("sample_min_LHE_HT", -1);
  data.add<int>("sample_use_pythia", 0);
  data.add<int>("sample_use_herwig", 0);
  data.add<int>("sample_use_madgraph", 0);

  data.add<int>("sample_useReclusteredJets", useReclusteredJets_);

//  // legacy labels
//  data.add<int>("fj_labelLegacy", 0);

  // JMAR label
  data.add<int>("fj_labelJMAR", 0);
  data.add<float>("fjJMAR_gen_pt", 0);
  data.add<float>("fjJMAR_gen_eta", 0);
  data.add<float>("fjJMAR_gen_phi", 0);
  data.add<int>("fjJMAR_gen_pdgid", 0);

  // gen-matched particle (top/W/etc.)
  data.add<float>("fj_gen_pt", 0);
  data.add<float>("fj_gen_eta", 0);
  data.add<float>("fj_gen_mass", 0);
  data.add<float>("fj_gen_deltaR", 999);

  // specific gen info for H --> aa --> bbbb decay
  data.add<float>("fj_gen_H_aa_bbbb_mass_a1", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_a2", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bb_mass_a2", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bb_mass_a1", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bbbar_mass_a2", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bbbar_mass_a1", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bb_dR_a2", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bb_dR_a1", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bbbar_dR_a2", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_bbbar_dR_a1", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_dR_max_b", -0.1);
  data.add<float>("fj_gen_H_aa_bbbb_pt_min_b", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_H", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_mass_H_calc", -1.0);

  data.add<float>("fj_gen_H_aa_bbbb_pt_a1", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_pt_a2", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_pt_b11", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_pt_b12", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_pt_b21", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_pt_b22", -1.0);
  data.add<float>("fj_gen_H_aa_bbbb_eta_a1", 999);
  data.add<float>("fj_gen_H_aa_bbbb_eta_a2", 999);
  data.add<float>("fj_gen_H_aa_bbbb_eta_b11", 999);
  data.add<float>("fj_gen_H_aa_bbbb_eta_b12", 999);
  data.add<float>("fj_gen_H_aa_bbbb_eta_b21", 999);
  data.add<float>("fj_gen_H_aa_bbbb_eta_b22", 999);
  data.add<float>("fj_gen_H_aa_bbbb_phi_a1", 999);
  data.add<float>("fj_gen_H_aa_bbbb_phi_a2", 999);
  data.add<float>("fj_gen_H_aa_bbbb_phi_b11", 999);
  data.add<float>("fj_gen_H_aa_bbbb_phi_b12", 999);
  data.add<float>("fj_gen_H_aa_bbbb_phi_b21", 999);
  data.add<float>("fj_gen_H_aa_bbbb_phi_b22", 999);

  // --- jet energy/mass regression ---
  data.add<float>("fj_genjet_pt", 0);
  data.add<float>("fj_genOverReco_pt", 1); // default to 1 if not gen-matched
  data.add<float>("fj_genOverReco_pt_null", 0); // default to 0 if not gen-matched
  data.add<float>("fj_genjet_mass", 0);
  data.add<float>("fj_genOverReco_mass", 1); // default to 1 if not gen-matched
  data.add<float>("fj_genOverReco_mass_null", 0); // default to 0 if not gen-matched
  data.add<float>("fj_genjet_sdmass", 0);
  data.add<float>("fj_genjet_sdmass_sqrt", 0);
  data.add<float>("fj_genjet_targetmass", 0);
  data.add<float>("fj_genOverReco_sdmass", 1); // default to 1 if not gen-matched
  data.add<float>("fj_genOverReco_sdmass_null", 0); // default to 0 if not gen-matched
  // ----------------------------------

  // fatjet kinematics
  data.add<float>("fj_pt", 0);
  data.add<float>("fj_eta", 0);
  data.add<float>("fj_phi", 0);
  data.add<float>("fj_mass", 0);

  // substructure
  data.add<float>("fj_tau1", 0);
  data.add<float>("fj_tau2", 0);
  data.add<float>("fj_tau3", 0);
  data.add<float>("fj_tau21", 0);
  data.add<float>("fj_tau32", 0);

  // soft drop
  data.add<float>("fj_sdmass", 0);
  data.add<float>("fj_sdmass_fromsubjets", 0);
  data.add<float>("fj_rho", 0);
  data.add<float>("fj_uncorrsdmass", 0);
  data.add<float>("fj_corrsdmass", 0);

  // subjets: soft drop gives up to 2 subjets
  data.add<float>("fj_n_sdsubjets", 0);

  data.add<float>("fj_sdsj1_pt", 0);
  data.add<float>("fj_sdsj1_eta", 0);
  data.add<float>("fj_sdsj1_phi", 0);
  data.add<float>("fj_sdsj1_mass", 0);
  data.add<float>("fj_sdsj1_csv", 0);
  data.add<float>("fj_sdsj1_ptD", 0);
  data.add<float>("fj_sdsj1_axis1", 0);
  data.add<float>("fj_sdsj1_axis2", 0);
  data.add<float>("fj_sdsj1_mult", 0);

  data.add<float>("fj_sdsj2_pt", 0);
  data.add<float>("fj_sdsj2_eta", 0);
  data.add<float>("fj_sdsj2_phi", 0);
  data.add<float>("fj_sdsj2_mass", 0);
  data.add<float>("fj_sdsj2_csv", 0);
  data.add<float>("fj_sdsj2_ptD", 0);
  data.add<float>("fj_sdsj2_axis1", 0);
  data.add<float>("fj_sdsj2_axis2", 0);
  data.add<float>("fj_sdsj2_mult", 0);

  // some variables used in a baseline tagger
  data.add<float>("fj_ptDR", 0);
  data.add<float>("fj_relptdiff", 0);
  data.add<float>("fj_sdn2", 0);

  //double-b
  data.add<float>("fj_doubleb", 0);

  //flavor info
  data.add<int>("fj_isBB", 0);
  data.add<int>("fj_isNonBB", 0);
  data.add<int>("fj_nbHadrons", 0);
  data.add<int>("fj_ncHadrons", 0);

}

bool FatJetInfoFiller::fill(const pat::Jet& jet, size_t jetidx, const JetHelper& jet_helper) {

  // JMAR label
  {
    auto jmar = fjmatch_.flavorJMAR(&jet, *genParticlesHandle, 0.75*jetR_);
    data.fill<int>("fj_labelJMAR", jmar.first);
    data.fill<float>("fjJMAR_gen_pt", jmar.second ? jmar.second->pt() : -999);
    data.fill<float>("fjJMAR_gen_eta", jmar.second ? jmar.second->eta() : -999);
    data.fill<float>("fjJMAR_gen_phi", jmar.second ? jmar.second->phi() : -999);
    data.fill<int>("fjJMAR_gen_pdgid", jmar.second ? jmar.second->pdgId() : -999);
  }

  // ----------------------------------------------------------------
  //  auto fjlabel = fjmatch_.flavorLabel(&jet, *genParticlesHandle, 0.6);
  auto fjlabel = fjmatch_.flavorLabel(&jet, *genParticlesHandle, jetR_);

  data.fill<int>("fj_label", fjlabel.first);

  data.fill<int>("fj_isTT",  fjlabel.first >= FatJetMatching::TT_all  && fjlabel.first < FatJetMatching::Top_all);
  data.fill<int>("fj_isTop", fjlabel.first >= FatJetMatching::Top_all && fjlabel.first < FatJetMatching::W_all);
  data.fill<int>("fj_isW",   fjlabel.first >= FatJetMatching::W_all && fjlabel.first < FatJetMatching::Z_all);
  data.fill<int>("fj_isZ",   fjlabel.first >= FatJetMatching::Z_all && fjlabel.first < FatJetMatching::H_all);
  data.fill<int>("fj_isH",   fjlabel.first >= FatJetMatching::H_all && fjlabel.first < FatJetMatching::QCD_all);
  data.fill<int>("fj_isISR", fjlabel.first == FatJetMatching::TT_ISR || fjlabel.first == FatJetMatching::Top_ISR ||
		             fjlabel.first == FatJetMatching::W_ISR || fjlabel.first == FatJetMatching::Z_ISR);
  data.fill<int>("fj_isQCD", fjlabel.first >= FatJetMatching::QCD_all);

  // veto unmatched jets in signal samples for training
  if (isTrainSample_ && !isQCDSample_ && fjlabel.first >= FatJetMatching::QCD_all)
    return false;

  int _label_NR_LF =  (fjlabel.first == FatJetMatching::TT_q || fjlabel.first == FatJetMatching::TT_q_q ||
		       fjlabel.first == FatJetMatching::W_c  || fjlabel.first == FatJetMatching::W_q ||
		       fjlabel.first == FatJetMatching::Z_c  || fjlabel.first == FatJetMatching::Z_q);
  int _label_NR_b =   (fjlabel.first == FatJetMatching::TT_b   || fjlabel.first == FatJetMatching::TT_bq ||
		       fjlabel.first == FatJetMatching::TT_b_q || fjlabel.first == FatJetMatching::TT_bq_q ||
		       fjlabel.first == FatJetMatching::Top_b || fjlabel.first == FatJetMatching::Top_bc ||
		       fjlabel.first == FatJetMatching::Top_bq || fjlabel.first == FatJetMatching::Top_btauH ||
		       fjlabel.first == FatJetMatching::Z_b);
  int _label_NR_bb =  (fjlabel.first == FatJetMatching::TT_b_b || fjlabel.first == FatJetMatching::TT_bq_b ||
		       fjlabel.first == FatJetMatching::TT_bq_bq);
  int _label_NR_lep = (fjlabel.first == FatJetMatching::TT_l    || fjlabel.first == FatJetMatching::TT_bl ||
		       fjlabel.first == FatJetMatching::TT_l_q  || fjlabel.first == FatJetMatching::TT_bl_q ||
		       fjlabel.first == FatJetMatching::TT_l_b  || fjlabel.first == FatJetMatching::TT_bl_b ||
		       fjlabel.first == FatJetMatching::TT_l_bq || fjlabel.first == FatJetMatching::TT_bl_bq ||
		       fjlabel.first == FatJetMatching::Top_bele || fjlabel.first == FatJetMatching::Top_bmu ||
		       fjlabel.first == FatJetMatching::Top_btauH);
  int _label_NR_ll =  (fjlabel.first == FatJetMatching::TT_l_l || fjlabel.first == FatJetMatching::TT_bl_l ||
		       fjlabel.first == FatJetMatching::TT_bl_bl);

  data.fill<int>("fj_isNonRes", _label_NR_LF+_label_NR_b+_label_NR_bb+_label_NR_lep);
  data.fill<int>("label_NR_LF",  _label_NR_LF);
  data.fill<int>("label_NR_b",   _label_NR_b);
  data.fill<int>("label_NR_bb",  _label_NR_bb);
  data.fill<int>("label_NR_lep", _label_NR_lep);
  data.fill<int>("label_NR_ll",  _label_NR_ll);

  data.fill<int>("label_TT_ISR",     fjlabel.first == FatJetMatching::TT_ISR);
  data.fill<int>("label_TT_q",       fjlabel.first == FatJetMatching::TT_q);
  data.fill<int>("label_TT_b",       fjlabel.first == FatJetMatching::TT_b);
  data.fill<int>("label_TT_bq",      fjlabel.first == FatJetMatching::TT_bq);
  data.fill<int>("label_TT_qq",      fjlabel.first == FatJetMatching::TT_qq);
  data.fill<int>("label_TT_bqq",     fjlabel.first == FatJetMatching::TT_bqq);
  data.fill<int>("label_TT_l",       fjlabel.first == FatJetMatching::TT_l);
  data.fill<int>("label_TT_bl",      fjlabel.first == FatJetMatching::TT_bl);
  data.fill<int>("label_TT_q_q",     fjlabel.first == FatJetMatching::TT_q_q);
  data.fill<int>("label_TT_b_q",     fjlabel.first == FatJetMatching::TT_b_q);
  data.fill<int>("label_TT_bq_q",    fjlabel.first == FatJetMatching::TT_bq_q);
  data.fill<int>("label_TT_qq_q",    fjlabel.first == FatJetMatching::TT_qq_q);
  data.fill<int>("label_TT_bqq_q",   fjlabel.first == FatJetMatching::TT_bqq_q);
  data.fill<int>("label_TT_l_q",     fjlabel.first == FatJetMatching::TT_l_q);
  data.fill<int>("label_TT_bl_q",    fjlabel.first == FatJetMatching::TT_bl_q);
  data.fill<int>("label_TT_b_b",     fjlabel.first == FatJetMatching::TT_b_b);
  data.fill<int>("label_TT_bq_b",    fjlabel.first == FatJetMatching::TT_bq_b);
  data.fill<int>("label_TT_qq_b",    fjlabel.first == FatJetMatching::TT_qq_b);
  data.fill<int>("label_TT_bqq_b",   fjlabel.first == FatJetMatching::TT_bqq_b);
  data.fill<int>("label_TT_l_b",     fjlabel.first == FatJetMatching::TT_l_b);
  data.fill<int>("label_TT_bl_b",    fjlabel.first == FatJetMatching::TT_bl_b);
  data.fill<int>("label_TT_bq_bq",   fjlabel.first == FatJetMatching::TT_bq_bq);
  data.fill<int>("label_TT_qq_bq",   fjlabel.first == FatJetMatching::TT_qq_bq);
  data.fill<int>("label_TT_bqq_bq",  fjlabel.first == FatJetMatching::TT_bqq_bq);
  data.fill<int>("label_TT_l_bq",    fjlabel.first == FatJetMatching::TT_l_bq);
  data.fill<int>("label_TT_bl_bq",   fjlabel.first == FatJetMatching::TT_bl_bq);
  data.fill<int>("label_TT_qq_qq",   fjlabel.first == FatJetMatching::TT_qq_qq);
  data.fill<int>("label_TT_bqq_qq",  fjlabel.first == FatJetMatching::TT_bqq_qq);
  data.fill<int>("label_TT_l_qq",    fjlabel.first == FatJetMatching::TT_l_qq);
  data.fill<int>("label_TT_bl_qq",   fjlabel.first == FatJetMatching::TT_bl_qq);
  data.fill<int>("label_TT_bqq_bqq", fjlabel.first == FatJetMatching::TT_bqq_bqq);
  data.fill<int>("label_TT_l_bqq",   fjlabel.first == FatJetMatching::TT_l_bqq);
  data.fill<int>("label_TT_bl_bqq",  fjlabel.first == FatJetMatching::TT_bl_bqq);
  data.fill<int>("label_TT_l_l",     fjlabel.first == FatJetMatching::TT_l_l);
  data.fill<int>("label_TT_bl_l",    fjlabel.first == FatJetMatching::TT_bl_l);
  data.fill<int>("label_TT_bl_bl",   fjlabel.first == FatJetMatching::TT_bl_bl);

  data.fill<int>("label_Top_bcq", fjlabel.first == FatJetMatching::Top_bcq);
  data.fill<int>("label_Top_bqq", fjlabel.first == FatJetMatching::Top_bqq);
  data.fill<int>("label_Top_bc",  fjlabel.first == FatJetMatching::Top_bc);
  data.fill<int>("label_Top_bq",  fjlabel.first == FatJetMatching::Top_bq);
  data.fill<int>("label_Top_bqt", fjlabel.first == FatJetMatching::Top_bq || fjlabel.first == FatJetMatching::Top_btauH);
  data.fill<int>("label_Top_bele",fjlabel.first == FatJetMatching::Top_bele);
  data.fill<int>("label_Top_bmu", fjlabel.first == FatJetMatching::Top_bmu);
  data.fill<int>("label_Top_btauH",fjlabel.first == FatJetMatching::Top_btauH);
  data.fill<int>("label_Top_btauL",fjlabel.first == FatJetMatching::Top_btauL);
  data.fill<int>("label_Top_bl",  fjlabel.first == FatJetMatching::Top_bele || fjlabel.first == FatJetMatching::Top_bmu);
  data.fill<int>("label_Top_blt", fjlabel.first == FatJetMatching::Top_bele || fjlabel.first == FatJetMatching::Top_bmu || fjlabel.first == FatJetMatching::Top_btauL);
  data.fill<int>("label_Top_b",fjlabel.first == FatJetMatching::Top_b);
  data.fill<int>("label_Top_ISR",fjlabel.first == FatJetMatching::Top_ISR);

  data.fill<int>("label_W_cq",    fjlabel.first == FatJetMatching::W_cq);
  data.fill<int>("label_W_qq",    fjlabel.first == FatJetMatching::W_qq);
  data.fill<int>("label_W_c",     fjlabel.first == FatJetMatching::W_c);
  data.fill<int>("label_W_q",     fjlabel.first == FatJetMatching::W_q);
  data.fill<int>("label_W_ISR",   fjlabel.first == FatJetMatching::W_ISR);

  data.fill<int>("label_Z_bb",    fjlabel.first == FatJetMatching::Z_bb);
  data.fill<int>("label_Z_cc",    fjlabel.first == FatJetMatching::Z_cc);
  data.fill<int>("label_Z_qq",    fjlabel.first == FatJetMatching::Z_qq);
  data.fill<int>("label_Z_b",     fjlabel.first == FatJetMatching::Z_b);
  data.fill<int>("label_Z_c",     fjlabel.first == FatJetMatching::Z_c);
  data.fill<int>("label_Z_q",     fjlabel.first == FatJetMatching::Z_q);
  data.fill<int>("label_Z_ISR",   fjlabel.first == FatJetMatching::Z_ISR);

  data.fill<int>("label_H_bb",    fjlabel.first == FatJetMatching::H_bb);
  data.fill<int>("label_H_cc",    fjlabel.first == FatJetMatching::H_cc);
  data.fill<int>("label_H_qq",    fjlabel.first == FatJetMatching::H_qq);
  data.fill<int>("label_H_qqqq",  fjlabel.first == FatJetMatching::H_qqqq);
  data.fill<int>("label_H_tautau",fjlabel.first == FatJetMatching::H_tautau);
  data.fill<int>("label_H_aa_bbbb",  fjlabel.first == FatJetMatching::H_aa_bbbb);
  data.fill<int>("label_H_aa_other", fjlabel.first == FatJetMatching::H_aa_other);

  data.fill<int>("label_QCD_bb",  fjlabel.first == FatJetMatching::QCD_bb);
  data.fill<int>("label_QCD_cc",  fjlabel.first == FatJetMatching::QCD_cc);
  data.fill<int>("label_QCD_b",   fjlabel.first == FatJetMatching::QCD_b);
  data.fill<int>("label_QCD_c",   fjlabel.first == FatJetMatching::QCD_c);
  data.fill<int>("label_QCD_others", fjlabel.first == FatJetMatching::QCD_others);
  data.fill<int>("label_QCD_BGen", sampleType_ == "QCD_BGen");
  data.fill<int>("label_QCD_bEnr", sampleType_ == "QCD_bEnr");
  data.fill<int>("label_QCD_Incl", sampleType_ == "QCD_Incl");

  data.fill<int>("sample_isQCD",  isQCDSample_);
  data.fill<int>("sample_min_LHE_HT", min_LHE_HT_);
  data.fill<int>("sample_use_pythia", sample_use_pythia_);
  data.fill<int>("sample_use_herwig", sample_use_herwig_);
  data.fill<int>("sample_use_madgraph", sample_use_madgraph_); // MG can be interfaced w/ either pythia or herwig

  // specific gen info for H --> aa --> bbbb decay
  // matching code taken from FatJetHelpers/src/FatJetMatching.cc
  double dR_max_b  = -0.1;
  double pt_min_b  = 9999;
  double mass_H    = -1.0;
  double mass_bb_mass_a2 = 9999;
  double mass_bb_mass_a1 = 9999;
  double mass_bbbar_mass_a2 = 9999;
  double mass_bbbar_mass_a1 = 9999;
  double dR_bb_min = 9999;
  double mass_bb_dR_a2 = 9999;
  double mass_bb_dR_a1 = 9999;
  double dR_bbbar_min = 9999;
  double mass_bbbar_dR_a2 = 9999;
  double mass_bbbar_dR_a1 = 9999;

  // declare the tlozentz vectors of the b, then the a, and H 
  // initialize the a vectors to 0 
  ROOT::Math::PtEtaPhiMVector bVec[2];    // b quarks from a boson decays
  ROOT::Math::PtEtaPhiMVector bbarVec[2]; // anti-b quarks from a boson decays
  ROOT::Math::PtEtaPhiMVector aVec[2];    // a bosons from b-bbar pairs
  ROOT::Math::PtEtaPhiMVector Higgs;      // Higgs constructed from a bosons
  ROOT::Math::PtEtaPhiMVector tmpVec;     // Temporary 4-vector for swapping values
  
  if ( (fjlabel.first == FatJetMatching::H_aa_bbbb ||
	fjlabel.first == FatJetMatching::H_aa_other) && fjlabel.second ) {
    mass_H = fjlabel.second->mass();
    for (unsigned idau=0; idau<fjlabel.second->numberOfDaughters(); ++idau){
      const auto *dau = dynamic_cast<const reco::GenParticle*>(fjlabel.second->daughter(idau));
      // H_aa_bbbb_mass_a1 = dau->mass(); // this is the read out mass. We will use calculated mass instead
      for (unsigned jdau=0; jdau<dau->numberOfDaughters(); ++jdau){
	const auto *gdau = dynamic_cast<const reco::GenParticle*>(dau->daughter(jdau));
	auto pdgid = gdau->pdgId();
	if (std::abs(pdgid) == ParticleID::p_b){
	  // make the tlorentz vector of the two b here. use jdau
	  if (pdgid > 0) {
	    bVec[idau].SetCoordinates(gdau->pt(), gdau->eta(), gdau->phi(), gdau->mass());
	  } else {
	    bbarVec[idau].SetCoordinates(gdau->pt(), gdau->eta(), gdau->phi(), gdau->mass());
	  }
	  if (gdau->pt() < pt_min_b) {
	    pt_min_b = gdau->pt();
	  }
	  if (reco::deltaR(*gdau, jet.p4()) > dR_max_b) {
	    dR_max_b = reco::deltaR(*gdau, jet.p4());
	  }
	}
      }
      // use info from the two b to fill in the tlorentz vector of a. use idau 
      aVec[idau] = bVec[idau] + bbarVec[idau];
    }
    Higgs = aVec[0] + aVec[1];

    // Swap a1 and a2 if mass(a2) is more than 2% larger than mass(a1)
    // or if they have the same mass and pT(a2) > pT(a1)
    bool mass_diff = (abs((aVec[1].M() - aVec[0].M()) / (aVec[1].M() + aVec[0].M())) > 0.01);
    if ( (     mass_diff  && (aVec[1].M()  > aVec[0].M())  ) ||
	 ((not mass_diff) && (aVec[1].Pt() > aVec[0].Pt()) ) ) {
      tmpVec = aVec[0];
      aVec[0] = aVec[1];
      aVec[1] = tmpVec;
      tmpVec = bVec[0];
      bVec[0] = bVec[1];
      bVec[1] = tmpVec;
      tmpVec = bbarVec[0];
      bbarVec[0] = bbarVec[1];
      bbarVec[1] = tmpVec;
    }

    // "a2" is b-bbar or b(bar)-b(bar) pair with minimum invariant mass or minimum dR separation
    // "a1" is the invariant mass of the other b-bbar or b(bar)-b(bar) pair
    for (uint i1=0; i1<2; i1++) {
      for (uint i2=0; i2<2; i2++) {
	uint j1 = (i1 == 0 ? 1 : 0);
	uint j2 = (i2 == 0 ? 1 : 0);
	if (mass_bbbar_mass_a2 > (bVec[i1]+bbarVec[i2]).M()) {
	  mass_bbbar_mass_a2   = (bVec[i1]+bbarVec[i2]).M();
	  mass_bbbar_mass_a1   = (bVec[j1]+bbarVec[j2]).M();
	}
	if (dR_bbbar_min > reco::deltaR(bVec[i1], bbarVec[i2])) {
	  dR_bbbar_min   = reco::deltaR(bVec[i1], bbarVec[i2]);
	  mass_bbbar_dR_a2 = (bVec[i1]+bbarVec[i2]).M();
	  mass_bbbar_dR_a1 = (bVec[j1]+bbarVec[j2]).M();
	}
	if (mass_bb_mass_a2 > mass_bbbar_mass_a2) {
	  mass_bb_mass_a2   = mass_bbbar_mass_a2;
	  mass_bb_mass_a1   = mass_bbbar_mass_a1;
	}
	if (dR_bb_min > dR_bbbar_min) {
	  dR_bb_min   = dR_bbbar_min;
	  mass_bb_dR_a2 = mass_bbbar_dR_a2;
	  mass_bb_dR_a1 = mass_bbbar_dR_a1;
	}
      }
    }
    if (mass_bb_mass_a2 > (bVec[0]+bVec[1]).M()) {
      mass_bb_mass_a2   = (bVec[0]+bVec[1]).M();
      mass_bb_mass_a1   = (bbarVec[0]+bbarVec[1]).M();
    }
    if (mass_bb_mass_a2 > (bbarVec[0]+bbarVec[1]).M()) {
      mass_bb_mass_a2   = (bbarVec[0]+bbarVec[1]).M();
      mass_bb_mass_a1   = (bVec[0]+bVec[1]).M();
    }
    if (dR_bb_min > reco::deltaR(bVec[0], bVec[1])) {
      dR_bb_min   = reco::deltaR(bVec[0], bVec[1]);
      mass_bb_dR_a2 = (bVec[0]+bVec[1]).M();
      mass_bb_dR_a1 = (bbarVec[0]+bbarVec[1]).M();
    }
    if (dR_bb_min > reco::deltaR(bbarVec[0], bbarVec[1])) {
      dR_bb_min   = reco::deltaR(bbarVec[0], bbarVec[1]);
      mass_bb_dR_a2 = (bbarVec[0]+bbarVec[1]).M();
      mass_bb_dR_a1 = (bVec[0]+bVec[1]).M();
    }

    data.fill<float>("fj_gen_H_aa_bbbb_mass_a1", aVec[0].M());
    data.fill<float>("fj_gen_H_aa_bbbb_mass_a2", aVec[1].M());
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bb_mass_a2", mass_bb_mass_a2);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bb_mass_a1", mass_bb_mass_a1);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bbbar_mass_a2", mass_bbbar_mass_a2);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bbbar_mass_a1", mass_bbbar_mass_a1);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bb_dR_a2", mass_bb_dR_a2);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bb_dR_a1", mass_bb_dR_a1);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bbbar_dR_a2", mass_bbbar_dR_a2);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_bbbar_dR_a1", mass_bbbar_dR_a1);
    data.fill<float>("fj_gen_H_aa_bbbb_dR_max_b", dR_max_b);
    data.fill<float>("fj_gen_H_aa_bbbb_pt_min_b", pt_min_b);
    data.fill<float>("fj_gen_H_aa_bbbb_mass_H_calc", Higgs.M());
    data.fill<float>("fj_gen_H_aa_bbbb_mass_H", mass_H);

    data.fill<float>("fj_gen_H_aa_bbbb_pt_a1",  aVec[0].Pt());
    data.fill<float>("fj_gen_H_aa_bbbb_pt_a2",  aVec[1].Pt());
    data.fill<float>("fj_gen_H_aa_bbbb_pt_b11", bVec[0].Pt());
    data.fill<float>("fj_gen_H_aa_bbbb_pt_b12", bbarVec[0].Pt());
    data.fill<float>("fj_gen_H_aa_bbbb_pt_b21", bVec[1].Pt());
    data.fill<float>("fj_gen_H_aa_bbbb_pt_b22", bbarVec[1].Pt());
    data.fill<float>("fj_gen_H_aa_bbbb_eta_a1",  aVec[0].Eta()    - Higgs.Eta());
    data.fill<float>("fj_gen_H_aa_bbbb_eta_a2",  aVec[1].Eta()    - Higgs.Eta());
    data.fill<float>("fj_gen_H_aa_bbbb_eta_b11", bVec[0].Eta()    - Higgs.Eta());
    data.fill<float>("fj_gen_H_aa_bbbb_eta_b12", bbarVec[0].Eta() - Higgs.Eta());
    data.fill<float>("fj_gen_H_aa_bbbb_eta_b21", bVec[1].Eta()    - Higgs.Eta());
    data.fill<float>("fj_gen_H_aa_bbbb_eta_b22", bbarVec[1].Eta() - Higgs.Eta()); // (targetAngle - myAngle + 540) % 360 - 180;
    data.fill<float>("fj_gen_H_aa_bbbb_phi_a1",  fmod(aVec[0].Phi() - Higgs.Phi() + 3*Geom::pi(), 2*Geom::pi()) - Geom::pi());
    data.fill<float>("fj_gen_H_aa_bbbb_phi_a2",  fmod(aVec[1].Phi() - Higgs.Phi() + 3*Geom::pi(), 2*Geom::pi()) - Geom::pi());
    data.fill<float>("fj_gen_H_aa_bbbb_phi_b11", fmod(bVec[0].Phi()    - Higgs.Phi() + 3*Geom::pi(), 2*Geom::pi()) - Geom::pi());
    data.fill<float>("fj_gen_H_aa_bbbb_phi_b12", fmod(bbarVec[0].Phi() - Higgs.Phi() + 3*Geom::pi(), 2*Geom::pi()) - Geom::pi());
    data.fill<float>("fj_gen_H_aa_bbbb_phi_b21", fmod(bVec[1].Phi()    - Higgs.Phi() + 3*Geom::pi(), 2*Geom::pi()) - Geom::pi());
    data.fill<float>("fj_gen_H_aa_bbbb_phi_b22", fmod(bbarVec[1].Phi() - Higgs.Phi() + 3*Geom::pi(), 2*Geom::pi()) - Geom::pi());
  } // End conditional: if ( (fjlabel.first == FatJetMatching::H_aa_bbbb || ...

  // gen-matched particle (top/W/etc.) WARNING: only has one top from ttbar events! - AWB 2024.03.26
  data.fill<float>("fj_gen_pt", fjlabel.second ? fjlabel.second->pt() : -999);
  data.fill<float>("fj_gen_eta", fjlabel.second ? fjlabel.second->eta() : -999);
  data.fill<float>("fj_gen_mass", (fjlabel.first < FatJetMatching::QCD_all && fjlabel.second) ? fjlabel.second->mass() : 0);
  data.fill<float>("fj_gen_deltaR", fjlabel.second ? reco::deltaR(jet, fjlabel.second->p4()) : 999);

  // ----------------------------------

  // fatjet kinematics
  data.fill<float>("fj_pt", jet.pt());
  data.fill<float>("fj_eta", jet.eta());
  data.fill<float>("fj_phi", jet.phi());
  data.fill<float>("fj_mass", jet.mass());

  // substructure
  float tau1 = jet.userFloat("Njettiness" + fjName +"Puppi:tau1");
  float tau2 = jet.userFloat("Njettiness" + fjName +"Puppi:tau2");
  float tau3 = jet.userFloat("Njettiness" + fjName +"Puppi:tau3");
  data.fill<float>("fj_tau1", tau1);
  data.fill<float>("fj_tau2", tau2);
  data.fill<float>("fj_tau3", tau3);
  data.fill<float>("fj_tau21", tau1 > 0 ? tau2/tau1 : 1.01);
  data.fill<float>("fj_tau32", tau2 > 0 ? tau3/tau2 : 1.01);

  // soft drop
  std::string name(fjName);
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  auto msd_uncorr = jet.userFloat(name +"PFJetsPuppiSoftDropMass");
  data.fill<float>("fj_sdmass", msd_uncorr);
  data.fill<float>("fj_sdmass_fromsubjets", jet.groomedMass());
  data.fill<float>("fj_rho", 2 * std::log(std::max(jet.groomedMass(), 0.01) / jet_helper.jet().pt())); // use corrected pt

  // subjets: soft drop gives up to 2 subjets
  const auto& subjets = jet_helper.getSubJets();

  data.fill<float>("fj_n_sdsubjets", subjets.size());
  auto sdcorr = jet_helper.getCorrectedPuppiSoftDropMass(subjets);
  data.fill<float>("fj_uncorrsdmass", sdcorr.first);
  data.fill<float>("fj_corrsdmass", sdcorr.second);

  if (subjets.size() > 0){
    const auto &sj1 = subjets.at(0);
    data.fill<float>("fj_sdsj1_pt", sj1->pt());
    data.fill<float>("fj_sdsj1_eta", sj1->eta());
    data.fill<float>("fj_sdsj1_phi", sj1->phi());
    data.fill<float>("fj_sdsj1_mass", sj1->mass());
    data.fill<float>("fj_sdsj1_csv", sj1->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));

    if (subjets.size() > 1){
      const auto &sj2 = subjets.at(1);
      data.fill<float>("fj_sdsj2_pt", sj2->pt());
      data.fill<float>("fj_sdsj2_eta", sj2->eta());
      data.fill<float>("fj_sdsj2_phi", sj2->phi());
      data.fill<float>("fj_sdsj2_mass", sj2->mass());
      data.fill<float>("fj_sdsj2_csv", sj2->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));

      // some variables used in a baseline tagger
      float deltaR = reco::deltaR(*sj1, *sj2);
      float var_sd_0 = sj2->pt()/(sj1->pt()+sj2->pt());
      data.fill<float>("fj_ptDR", jet.pt() * deltaR);
      data.fill<float>("fj_relptdiff", std::abs(sj1->pt()-sj2->pt()) / jet.pt());
      data.fill<float>("fj_sdn2", var_sd_0/std::pow(deltaR,-2));
    }
  }

  // ----------------------------------------------------------------

  // --- jet energy/mass regression ---
  const auto *genjet = jet_helper.genjetWithNu();
  if (genjet){
    // jet here points to the uncorrected jet
    data.fill<float>("fj_genjet_pt", genjet->pt());
    data.fill<float>("fj_genOverReco_pt", catchInfs(genjet->pt() / jet.pt(), 1));
    data.fill<float>("fj_genOverReco_pt_null", catchInfs(genjet->pt() / jet.pt(), 0));
    data.fill<float>("fj_genjet_mass", genjet->mass());
    data.fill<float>("fj_genOverReco_mass", catchInfs(genjet->mass() / jet.mass(), 1));
    data.fill<float>("fj_genOverReco_mass_null", catchInfs(genjet->mass() / jet.mass(), 0));
  }
  const auto *sdgenjet = jet_helper.genjetWithNuSoftDrop();
  if (sdgenjet){
    // jet here points to the uncorrected jet
    auto pos = [](double x){ return x<0 ? 0 : x; };
    data.fill<float>("fj_genjet_sdmass", pos(sdgenjet->mass()));
    data.fill<float>("fj_genjet_sdmass_sqrt", std::sqrt(pos(sdgenjet->mass())));
    data.fill<float>("fj_genjet_targetmass", (fjlabel.first < FatJetMatching::QCD_all && fjlabel.second) ? fjlabel.second->mass() : pos(sdgenjet->mass()) );
    data.fill<float>("fj_genOverReco_sdmass", catchInfs(pos(sdgenjet->mass()) / pos(msd_uncorr), 1));
    data.fill<float>("fj_genOverReco_sdmass_null", catchInfs(pos(sdgenjet->mass()) / pos(msd_uncorr), 0));
  }


  // --------
  // double-b

  // const auto *bdsvTagInfo = jet.tagInfoBoostedDoubleSV(fjTagInfoName);
  // assert(bdsvTagInfo);
  // const auto &vars = bdsvTagInfo->taggingVariables();

  data.fill<float>("fj_doubleb", jet.bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"));

  //flavor info
  data.fill<int>("fj_isBB", jet.jetFlavourInfo().getbHadrons().size() >= 2);
  data.fill<int>("fj_isNonBB", jet.jetFlavourInfo().getbHadrons().size() < 2);
  data.fill<int>("fj_nbHadrons", jet.jetFlavourInfo().getbHadrons().size());
  data.fill<int>("fj_ncHadrons", jet.jetFlavourInfo().getcHadrons().size());

  //double-b inputs
  // data.fill<float>("fj_z_ratio", vars.get(reco::btau::z_ratio));
  // data.fill<float>("fj_trackSipdSig_3", vars.get(reco::btau::trackSip3dSig_3));
  // data.fill<float>("fj_trackSipdSig_2", vars.get(reco::btau::trackSip3dSig_2));
  // data.fill<float>("fj_trackSipdSig_1", vars.get(reco::btau::trackSip3dSig_1));
  // data.fill<float>("fj_trackSipdSig_0", vars.get(reco::btau::trackSip3dSig_0));
  // data.fill<float>("fj_trackSipdSig_1_0", vars.get(reco::btau::tau2_trackSip3dSig_0));
  // data.fill<float>("fj_trackSipdSig_0_0", vars.get(reco::btau::tau1_trackSip3dSig_0));
  // data.fill<float>("fj_trackSipdSig_1_1", vars.get(reco::btau::tau2_trackSip3dSig_1));
  // data.fill<float>("fj_trackSipdSig_0_1", vars.get(reco::btau::tau1_trackSip3dSig_1));
  // data.fill<float>("fj_trackSip2dSigAboveCharm_0", vars.get(reco::btau::trackSip2dSigAboveCharm));
  // data.fill<float>("fj_trackSip2dSigAboveBottom_0", vars.get(reco::btau::trackSip2dSigAboveBottom_0));
  // data.fill<float>("fj_trackSip2dSigAboveBottom_1", vars.get(reco::btau::trackSip2dSigAboveBottom_1));
  // data.fill<float>("fj_tau1_trackEtaRel_0", vars.get(reco::btau::tau2_trackEtaRel_0));
  // data.fill<float>("fj_tau1_trackEtaRel_1", vars.get(reco::btau::tau2_trackEtaRel_1));
  // data.fill<float>("fj_tau1_trackEtaRel_2", vars.get(reco::btau::tau2_trackEtaRel_2));
  // data.fill<float>("fj_tau0_trackEtaRel_0", vars.get(reco::btau::tau1_trackEtaRel_0));
  // data.fill<float>("fj_tau0_trackEtaRel_1", vars.get(reco::btau::tau1_trackEtaRel_1));
  // data.fill<float>("fj_tau0_trackEtaRel_2", vars.get(reco::btau::tau1_trackEtaRel_2));
  // data.fill<float>("fj_tau_vertexMass_0", vars.get(reco::btau::tau1_vertexMass));
  // data.fill<float>("fj_tau_vertexEnergyRatio_0", vars.get(reco::btau::tau1_vertexEnergyRatio));
  // data.fill<float>("fj_tau_vertexDeltaR_0", vars.get(reco::btau::tau1_vertexDeltaR));
  // data.fill<float>("fj_tau_flightDistance2dSig_0", vars.get(reco::btau::tau1_flightDistance2dSig));
  // data.fill<float>("fj_tau_vertexMass_1", vars.get(reco::btau::tau2_vertexMass));
  // data.fill<float>("fj_tau_vertexEnergyRatio_1", vars.get(reco::btau::tau2_vertexEnergyRatio));
  // data.fill<float>("fj_tau_flightDistance2dSig_1", vars.get(reco::btau::tau2_flightDistance2dSig));
  // data.fill<float>("fj_jetNTracks", vars.get(reco::btau::jetNTracks));
  // data.fill<float>("fj_nSV", vars.get(reco::btau::jetNSecondaryVertices));

  return true;
}

} /* namespace deepntuples */

