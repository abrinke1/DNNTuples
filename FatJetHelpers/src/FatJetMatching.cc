/*
 * FatJetMatching.cc
 *
 *  Created on: Feb 1, 2017
 *      Author: hqu
 */

#include "DeepNTuples/FatJetHelpers/interface/FatJetMatching.h"

#include <unordered_set>
#include "TString.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace deepntuples;

std::pair<FatJetMatching::FatJetFlavor, const reco::GenParticle*> FatJetMatching::flavorJMAR(const pat::Jet* jet,
    const reco::GenParticleCollection& genParticles, double genRadius) {

  processed_.clear();

  if (debug_) {
    std::cout << "\n=======\nJet (energy, pT, eta, phi) = "
        << jet->energy() << ", " << jet->pt() << ", " << jet->eta() << ", " << jet->phi()
        << std::endl << std::endl;
    printGenInfoHeader();
    for (unsigned ipart = 0; ipart<genParticles.size(); ++ipart){
      printGenParticleInfo(&genParticles[ipart], ipart);
    }
  }

  for (unsigned ipart = 0; ipart<genParticles.size(); ++ipart){
    const auto *gp = &genParticles[ipart];

    if (processed_.count(gp)) continue;
    processed_.insert(gp);

    auto pdgid = std::abs(gp->pdgId());
    if (pdgid == ParticleID::p_t){
      // top
      auto top = getFinal(gp);
      // find the W and test if it's hadronic
      const reco::GenParticle *w_from_top = nullptr, *b_from_top = nullptr;
      for (const auto &dau : top->daughterRefVector()){
        if (std::abs(dau->pdgId()) == ParticleID::p_Wplus){
          w_from_top = getFinal(&(*dau));
        }else if (std::abs(dau->pdgId()) <= ParticleID::p_b){
          // ! use <= p_b ! -- can also have charms etc.
          // for quarks use the first one in the decay chain
          b_from_top = dynamic_cast<const reco::GenParticle*>(&(*dau));
        }
      }
      if (!w_from_top || !b_from_top) throw std::logic_error("[FatJetMatching::flavor] Cannot find b or W from top decay: "+std::to_string(ipart));
      if (isHadronic(w_from_top)) {
        if (debug_){
          using namespace std;
          cout << "jet: " << jet->polarP4() << endl;
          cout << "top: "; printGenParticleInfo(top, -1);
          cout << "b:   "; printGenParticleInfo(b_from_top, -1);
          cout << "W:   "; printGenParticleInfo(w_from_top, -1);
        }

        double dr_jet_top = reco::deltaR(jet->p4(), top->p4());
        double dr_top_wdaus = maxDeltaRToDaughterQuarks(top, w_from_top);
        double dr_top_b     = reco::deltaR(top->p4(), b_from_top->p4());
        if (debug_){
          using namespace std;
          cout << "deltaR(jet, top)   : " << dr_jet_top << endl;
          cout << "deltaR(top, b)     : " << dr_top_b << endl;
          cout << "deltaR(top, w daus): " << dr_top_wdaus << endl;
        }
        // top
        if (dr_top_wdaus < genRadius && dr_top_b < genRadius && dr_jet_top < genRadius) return std::make_pair(FatJetFlavor::Top, top);

        double dr_jet_w = reco::deltaR(jet->p4(), w_from_top->p4());
        double dr_w = maxDeltaRToDaughterQuarks(w_from_top, w_from_top);
        if (debug_){
          using namespace std;
          cout << "deltaR(jet, w)     : " << dr_jet_w << endl;
          cout << "deltaR(w, w_daus)  : " << dr_w << endl;
        }
        if (dr_w < genRadius && dr_jet_w < genRadius) return std::make_pair(FatJetFlavor::W, w_from_top);
      }
    }else if (pdgid == ParticleID::p_h0) {
      // Higgs
      auto h = getFinal(gp);
      if (isHadronic(h)) {
        if (debug_){
          using namespace std;
          cout << "jet: " << jet->polarP4() << endl;
          cout << "H:   "; printGenParticleInfo(h, -1);
        }
        double dr_jet_h = reco::deltaR(jet->p4(), h->p4());
        double dr_hdaus = maxDeltaRToDaughterQuarks(h, h); // only works for h->bb??
        if (debug_){
          using namespace std;
          cout << "deltaR(jet, H)   : " << dr_jet_h << endl;
          cout << "deltaR(h, h daus): " << dr_hdaus << endl;
        }
        if (dr_hdaus < genRadius && dr_jet_h < genRadius) return std::make_pair(FatJetFlavor::H, h);
      }
    }else if (pdgid == ParticleID::p_Wplus){
      // W: not from top, or top not in jet cone
      auto w = getFinal(gp);
      if (isHadronic(w)) {
        if (debug_){
          using namespace std;
          cout << "jet: " << jet->polarP4() << endl;
          cout << "W:   "; printGenParticleInfo(w, -1);
        }
        double dr_jet_w = reco::deltaR(jet->p4(), w->p4());
        double dr_wdaus = maxDeltaRToDaughterQuarks(w, w);
        if (debug_){
          using namespace std;
          cout << "deltaR(jet, w)   : " << dr_jet_w << endl;
          cout << "deltaR(w, w daus): " << dr_wdaus << endl;
        }
        if (dr_wdaus < genRadius && dr_jet_w < genRadius) return std::make_pair(FatJetFlavor::W, w);

      }
    }else if (pdgid == ParticleID::p_Z0) {
      // Z
      auto z = getFinal(gp);
      if (isHadronic(z)) {
        if (debug_){
          using namespace std;
          cout << "jet: " << jet->polarP4() << endl;
          cout << "Z:   "; printGenParticleInfo(z, -1);
        }
        double dr_jet_z = reco::deltaR(jet->p4(), z->p4());
        double dr_zdaus = maxDeltaRToDaughterQuarks(z, z);
        if (debug_){
          using namespace std;
          cout << "deltaR(jet, Z)   : " << dr_jet_z << endl;
          cout << "deltaR(Z, Z daus): " << dr_zdaus << endl;
        }
        if (dr_zdaus < genRadius && dr_jet_z < genRadius) return std::make_pair(FatJetFlavor::Z, z);
      }
    }else {
      // ?
    }
  }

  if (genParticles.size() != processed_.size())
    throw std::logic_error("[FatJetMatching::flavor] Not all genParticles are processed!");

  const reco::GenParticle *parton = nullptr;
  double minDR = 999;
  for (const auto &gp : genParticles){
    if (gp.status() != 23) continue;
    auto pdgid = std::abs(gp.pdgId());
    if (!(pdgid<ParticleID::p_t || pdgid==ParticleID::p_g)) continue;
    auto dr = reco::deltaR(gp, *jet);
    if (dr<genRadius && dr<minDR){
      minDR = dr;
      parton = &gp;
    }
  }
  if (debug_){
    using namespace std;
    if (parton){
      cout << "parton"; printGenParticleInfo(parton, -1);
      cout << "dr(jet, parton): " << minDR << endl;
    }
  }

  return std::make_pair(FatJetFlavor::Default, parton);

}



// Assign label based on GEN particles matched to jet within radius distR
std::pair<FatJetMatching::FatJetLabel, const reco::GenParticle*> FatJetMatching::flavorLabel(const pat::Jet* jet,
    const reco::GenParticleCollection& genParticles, double distR) {

  processed_.clear();

  if (debug_) {
    std::cout << "\n=======\nJet (energy, pT, eta, phi) = "
        << jet->energy() << ", " << jet->pt() << ", " << jet->eta() << ", " << jet->phi()
        << std::endl << std::endl;
    printGenInfoHeader();
    for (unsigned ipart = 0; ipart<genParticles.size(); ++ipart){
      printGenParticleInfo(&genParticles[ipart], ipart);
    }
  }

  // Loop over all GEN particles, match to one initial state particle (or ttbar pair)
  // Priority ttbar > top > Higgs > W > Z > QCD
  // WARNING! Only supports WtoQQ and ZtoQQ samples, and undefined behavior
  //          with multiple initial state particles (e.g. WH) - AWB 2024.03.26
  for (unsigned ipart = 0; ipart<genParticles.size(); ++ipart){
    const auto *gp = &genParticles[ipart];

    if (processed_.count(gp)) continue;
    processed_.insert(gp);

    auto pdgid = std::abs(gp->pdgId());
    if (pdgid == ParticleID::p_t){
      // If we find both top and anti-top, categorize as ttbar
      for (unsigned jpart = 0; jpart<genParticles.size(); ++jpart){
	const auto *gpb = &genParticles[jpart];
	if (gp->pdgId() + gpb->pdgId() == 0) {
	  auto result = ttbar_label(jet, gp, gpb, distR);
	  if (result.first != FatJetLabel::Invalid){
	    return result;
	  }
	}
      }
      // If only one top quark, categorize as top
      auto result = top_label(jet, gp, distR);
      if (result.first != FatJetLabel::Invalid){
        return result;
      }
    }else if (pdgid == ParticleID::p_h0){
      auto result = higgs_label(jet, gp, distR);
      if (result.first != FatJetLabel::Invalid){
        return result;
      }
    }else if (pdgid == ParticleID::p_Wplus){
      auto result = w_label(jet, gp, distR);
      if (result.first != FatJetLabel::Invalid){
        return result;
      }
    }else if (pdgid == ParticleID::p_Z0){
      auto result = z_label(jet, gp, distR);
      if (result.first != FatJetLabel::Invalid){
        return result;
      }
    }
  }

  if (genParticles.size() != processed_.size())
    throw std::logic_error("[FatJetMatching::flavor] Not all genParticles are processed!");

  return qcd_label(jet, genParticles, distR);

}


void FatJetMatching::printGenInfoHeader() const {
  using namespace std;
  cout    << right << setw(6) << "#" << " " << setw(10) << "pdgId"
      << "  " << "Chg" << "  " << setw(10) << "Mass" << "  " << setw(48) << " Momentum"
      << left << "  " << setw(10) << "Mothers" << " " << setw(30) << "Daughters" << endl;
}

void FatJetMatching::printGenParticleInfo(const reco::GenParticle* genParticle, const int idx) const {
  using namespace std;
  cout  << right << setw(3) << genParticle->status();
  cout  << right << setw(3) << idx << " " << setw(10) << genParticle->pdgId() << "  ";
  cout  << right << "  " << setw(3) << genParticle->charge() << "  " << TString::Format("%10.3g", genParticle->mass() < 1e-5 ? 0 : genParticle->mass());
  cout  << left << setw(50) << TString::Format("  (E=%6.4g pT=%6.4g eta=%7.3g phi=%7.3g)", genParticle->energy(), genParticle->pt(), genParticle->eta(), genParticle->phi());

  TString                     mothers;
  for (unsigned int iMom = 0; iMom < genParticle->numberOfMothers(); ++iMom) {
    if (mothers.Length())     mothers        += ",";
    mothers   += genParticle->motherRef(iMom).key();
  }
  cout << "  " << setw(10) << mothers;
  TString                     daughters;
  for (unsigned int iDau = 0; iDau < genParticle->numberOfDaughters(); ++iDau) {
    if (daughters.Length())   daughters      += ",";
    daughters += genParticle->daughterRef(iDau).key();
  }
  cout << " " << setw(30) << daughters << endl;
}

const reco::GenParticle* FatJetMatching::getFinal(const reco::GenParticle* particle) {
  // will mark intermediate particles as processed
  if (!particle) return nullptr;
  processed_.insert(particle);
  const reco::GenParticle *final = particle;

  while (final->numberOfDaughters()) {
    const reco::GenParticle *chain = nullptr;
    for (unsigned idau = 0; idau < final->numberOfDaughters(); ++idau){
      if (final->daughter(idau)->pdgId() == particle->pdgId()) {
        chain = dynamic_cast<const reco::GenParticle*>(final->daughter(idau));
        processed_.insert(chain);
        break;
      }
    }
    if (!chain) break;
    final = chain;
  }
  return final;
}

bool FatJetMatching::isHadronic(const reco::GenParticle* particle) const {
  // particle needs to be the final version before decay
  if (!particle) throw std::invalid_argument("[FatJetMatching::isHadronic()] Null particle!");
  for(const auto &dau : particle->daughterRefVector()){
    auto pdgid = std::abs(dau->pdgId());
    if (pdgid >= ParticleID::p_d && pdgid <= ParticleID::p_b) return true;
  }
  return false;
}

std::vector<const reco::GenParticle*> FatJetMatching::getDaughterQuarks(const reco::GenParticle* particle) {
  std::vector<const reco::GenParticle*> daughters;

  for (unsigned i=0; i<particle->numberOfDaughters(); ++i){
    const auto *dau = dynamic_cast<const reco::GenParticle*>(particle->daughter(i));
    auto pdgid = std::abs(dau->pdgId());
    if (pdgid >= ParticleID::p_d && pdgid <= ParticleID::p_b){
      daughters.push_back(dau);
    }
  }

  return daughters;
}

std::pair<FatJetMatching::FatJetLabel,const reco::GenParticle*> FatJetMatching::ttbar_label(const pat::Jet* jet, const reco::GenParticle *partonA, const reco::GenParticle *partonB, double distR)
{

  // ttbar label based on individual top labels
  auto labA = top_label(jet, partonA, distR).first;
  auto labB = top_label(jet, partonB, distR).first;
  auto topA = getFinal(partonA);

  // Count "standard" top labels
  int n_ISR = ((labA == FatJetLabel::W_ISR)   + (labB == FatJetLabel::W_ISR) +
	       (labA == FatJetLabel::Top_ISR) + (labB == FatJetLabel::Top_ISR));
  int n_q   = ((labA == FatJetLabel::W_c || labA == FatJetLabel::W_q || labA == FatJetLabel::Top_tauH) +
	       (labB == FatJetLabel::W_c || labB == FatJetLabel::W_q || labB == FatJetLabel::Top_tauH));
  int n_b   = ((labA == FatJetLabel::Top_b) + (labB == FatJetLabel::Top_b));
  int n_bq  = ((labA == FatJetLabel::Top_bc || labA == FatJetLabel::Top_bq || labA == FatJetLabel::Top_btauH) +
	       (labB == FatJetLabel::Top_bc || labB == FatJetLabel::Top_bq || labB == FatJetLabel::Top_btauH));
  int n_qq  = ((labA == FatJetLabel::W_cq || labA == FatJetLabel::W_qq) +
	       (labB == FatJetLabel::W_cq || labB == FatJetLabel::W_qq));
  int n_bqq = ((labA == FatJetLabel::Top_bcq || labA == FatJetLabel::Top_bqq) +
	       (labB == FatJetLabel::Top_bcq || labB == FatJetLabel::Top_bqq));
  int n_l   = ((labA == FatJetLabel::Top_ele || labA == FatJetLabel::Top_mu || labA == FatJetLabel::Top_tauL) +
	       (labB == FatJetLabel::Top_ele || labB == FatJetLabel::Top_mu || labB == FatJetLabel::Top_tauL));
  int n_bl  = ((labA == FatJetLabel::Top_bele || labA == FatJetLabel::Top_bmu || labA == FatJetLabel::Top_btauL) +
	       (labB == FatJetLabel::Top_bele || labB == FatJetLabel::Top_bmu || labB == FatJetLabel::Top_btauL));

  // Make TT label corresponding to both top quarks
  if  (n_ISR == 2)                return std::make_pair(FatJetLabel::TT_ISR, topA);
  if ((n_ISR == 1) && (n_q == 1))   return std::make_pair(FatJetLabel::TT_q, topA);
  if ((n_ISR == 1) && (n_b == 1))   return std::make_pair(FatJetLabel::TT_b, topA);
  if ((n_ISR == 1) && (n_bq == 1))  return std::make_pair(FatJetLabel::TT_bq, topA);
  if ((n_ISR == 1) && (n_qq == 1))  return std::make_pair(FatJetLabel::TT_qq, topA);
  if ((n_ISR == 1) && (n_bqq == 1)) return std::make_pair(FatJetLabel::TT_bqq, topA);
  if ((n_ISR == 1) && (n_l == 1))   return std::make_pair(FatJetLabel::TT_l, topA);
  if ((n_ISR == 1) && (n_bl == 1))  return std::make_pair(FatJetLabel::TT_bl, topA);
  if  (n_q == 2)                  return std::make_pair(FatJetLabel::TT_q_q, topA);
  if ((n_q == 1) && (n_b == 1))   return std::make_pair(FatJetLabel::TT_b_q, topA);
  if ((n_q == 1) && (n_bq == 1))  return std::make_pair(FatJetLabel::TT_bq_q, topA);
  if ((n_q == 1) && (n_qq == 1))  return std::make_pair(FatJetLabel::TT_qq_q, topA);
  if ((n_q == 1) && (n_bqq == 1)) return std::make_pair(FatJetLabel::TT_bqq_q, topA);
  if ((n_q == 1) && (n_l == 1))   return std::make_pair(FatJetLabel::TT_l_q, topA);
  if ((n_q == 1) && (n_bl == 1))  return std::make_pair(FatJetLabel::TT_bl_q, topA);
  if  (n_b == 2)                  return std::make_pair(FatJetLabel::TT_b_b, topA);
  if ((n_b == 1) && (n_bq == 1))  return std::make_pair(FatJetLabel::TT_bq_b, topA);
  if ((n_b == 1) && (n_qq == 1))  return std::make_pair(FatJetLabel::TT_qq_b, topA);
  if ((n_b == 1) && (n_bqq == 1)) return std::make_pair(FatJetLabel::TT_bqq_b, topA);
  if ((n_b == 1) && (n_l == 1))   return std::make_pair(FatJetLabel::TT_l_b, topA);
  if ((n_b == 1) && (n_bl == 1))  return std::make_pair(FatJetLabel::TT_bl_b, topA);
  if  (n_bq == 2)                  return std::make_pair(FatJetLabel::TT_bq_bq, topA);
  if ((n_bq == 1) && (n_qq == 1))  return std::make_pair(FatJetLabel::TT_qq_bq, topA);
  if ((n_bq == 1) && (n_bqq == 1)) return std::make_pair(FatJetLabel::TT_bqq_bq, topA);
  if ((n_bq == 1) && (n_l == 1))   return std::make_pair(FatJetLabel::TT_l_bq, topA);
  if ((n_bq == 1) && (n_bl == 1))  return std::make_pair(FatJetLabel::TT_bl_bq, topA);
  if  (n_qq == 2)                  return std::make_pair(FatJetLabel::TT_qq_qq, topA);
  if ((n_qq == 1) && (n_bqq == 1)) return std::make_pair(FatJetLabel::TT_bqq_qq, topA);
  if ((n_qq == 1) && (n_l == 1))   return std::make_pair(FatJetLabel::TT_l_qq, topA);
  if ((n_qq == 1) && (n_bl == 1))  return std::make_pair(FatJetLabel::TT_bl_qq, topA);
  if  (n_bqq == 2)                 return std::make_pair(FatJetLabel::TT_bqq_bqq, topA);
  if ((n_bqq == 1) && (n_l == 1))  return std::make_pair(FatJetLabel::TT_l_bqq, topA);
  if ((n_bqq == 1) && (n_bl == 1)) return std::make_pair(FatJetLabel::TT_bl_bqq, topA);
  if  (n_l == 2)                 return std::make_pair(FatJetLabel::TT_l_l, topA);
  if ((n_l == 1) && (n_bl == 1)) return std::make_pair(FatJetLabel::TT_bl_l, topA);
  if  (n_bl == 2)              return std::make_pair(FatJetLabel::TT_bl_bl, topA);

  std::cout << "[FatJetMatching::ttbar_label] Top labels = " << labA << " and " << labB << std::endl;
  throw std::logic_error("[FatJetMatching::ttbar_label] Cannot find valid ttbar matching! See above.");

  return std::make_pair(FatJetLabel::Invalid, nullptr);
} // End function FatJetMatching::ttbar_label

std::pair<FatJetMatching::FatJetLabel,const reco::GenParticle*> FatJetMatching::top_label(const pat::Jet* jet, const reco::GenParticle *parton, double distR)
{

  // top
  auto top = getFinal(parton);
  // find the W and test if it's hadronic
  const reco::GenParticle *w_from_top = nullptr, *b_from_top = nullptr;
  for (const auto &dau : top->daughterRefVector()){
    if (std::abs(dau->pdgId()) == ParticleID::p_Wplus){
      w_from_top = getFinal(&(*dau));
    }else if (std::abs(dau->pdgId()) <= ParticleID::p_b){
      // ! use <= p_b ! -- can also have charms etc.
      b_from_top = dynamic_cast<const reco::GenParticle*>(&(*dau));
    }
  }
  if (!w_from_top || !b_from_top) throw std::logic_error("[FatJetMatching::top_label] Cannot find b or W from top decay!");

  if (isHadronic(w_from_top)) {
    if (debug_){
      using namespace std;
      cout << "jet: " << jet->polarP4() << endl;
      cout << "top: "; printGenParticleInfo(top, -1);
      cout << "b:   "; printGenParticleInfo(b_from_top, -1);
      cout << "W:   "; printGenParticleInfo(w_from_top, -1);
    }

    auto wdaus = getDaughterQuarks(w_from_top);
    if (wdaus.size() < 2) throw std::logic_error("[FatJetMatching::top_label] W decay has less than 2 quarks!");
    {
      double dr_b     = reco::deltaR(jet->p4(), b_from_top->p4());
      double dr_q1    = reco::deltaR(jet->p4(), wdaus.at(0)->p4());
      double dr_q2    = reco::deltaR(jet->p4(), wdaus.at(1)->p4());
      if (dr_q1 > dr_q2){
        // swap q1 and q2 so that dr_q1<=dr_q2
        std::swap(dr_q1, dr_q2);
        std::swap(wdaus.at(0), wdaus.at(1));
      }

      if (debug_){
        using namespace std;
        cout << "deltaR(jet, b)     : " << dr_b << endl;
        cout << "deltaR(jet, q1)    : " << dr_q1 << endl;
        cout << "deltaR(jet, q2)    : " << dr_q2 << endl;
      }

      if (dr_b < distR){
        auto pdgid_q1 = std::abs(wdaus.at(0)->pdgId());
        auto pdgid_q2 = std::abs(wdaus.at(1)->pdgId());
        if (debug_){
          using namespace std;
          cout << "pdgid(q1)        : " << pdgid_q1 << endl;
          cout << "pdgid(q2)        : " << pdgid_q2 << endl;
        }

        if (dr_q1<distR && dr_q2<distR){
          if (pdgid_q1 >= ParticleID::p_c || pdgid_q2 >= ParticleID::p_c) {
            return std::make_pair(FatJetLabel::Top_bcq, top);
          }
          else {
            return std::make_pair(FatJetLabel::Top_bqq, top);
          }
        }else if (dr_q1<distR && dr_q2>=distR){
          if (pdgid_q1 >= ParticleID::p_c){
            return std::make_pair(FatJetLabel::Top_bc, top);
          }else{
            return std::make_pair(FatJetLabel::Top_bq, top);
          }
        }else{
	  return std::make_pair(FatJetLabel::Top_b, top);
        }
      }else{
        // test for W if dr(b, jet) > distR
        return w_label(jet, w_from_top, distR);
      }
    }
  } else {
    // leptonic W
    if (debug_){
      using namespace std;
      cout << "jet: " << jet->polarP4() << endl;
      cout << "top: "; printGenParticleInfo(top, -1);
      cout << "b:   "; printGenParticleInfo(b_from_top, -1);
      cout << "W:   "; printGenParticleInfo(w_from_top, -1);
    }

    const reco::GenParticle* lep = nullptr;
    for (unsigned i=0; i<w_from_top->numberOfDaughters(); ++i){
      const auto *dau = dynamic_cast<const reco::GenParticle*>(w_from_top->daughter(i));
      auto pdgid = std::abs(dau->pdgId());
      if (pdgid == ParticleID::p_eminus || pdgid == ParticleID::p_muminus || pdgid == ParticleID::p_tauminus){
        // use final version here!
        lep = getFinal(dau); break;
      }
    }

    if (!lep) throw std::logic_error("[FatJetMatching::top_label] Cannot find charged lepton from leptonic W decay!");

    double dr_b     = reco::deltaR(jet->p4(), b_from_top->p4());
    double dr_l     = reco::deltaR(jet->p4(), lep->p4());
    if (debug_){
      using namespace std;
      cout << "deltaR(jet, b)     : " << dr_b << endl;
      cout << "deltaR(jet, l)     : " << dr_l << endl;
      cout << "pdgid(l)           : " << lep->pdgId() << endl;
    }

    auto lep_id = std::abs(lep->pdgId());
    if (dr_b < distR && dr_l < distR){
      if (lep_id == ParticleID::p_eminus){
        return std::make_pair(FatJetLabel::Top_bele, top);
      } else if (lep_id == ParticleID::p_muminus){
        return std::make_pair(FatJetLabel::Top_bmu, top);
      } else if (lep_id == ParticleID::p_tauminus){
	for (unsigned kdau=0; kdau < lep->numberOfDaughters(); ++kdau){
	  if ( std::abs(lep->daughter(kdau)->pdgId()) == ParticleID::p_eminus ||
	       std::abs(lep->daughter(kdau)->pdgId()) == ParticleID::p_muminus )
	    return std::make_pair(FatJetLabel::Top_btauL, top);
	}
	return std::make_pair(FatJetLabel::Top_btauH, top);
      }
    }
    else if (dr_l < distR){
      if (lep_id == ParticleID::p_eminus){
        return std::make_pair(FatJetLabel::Top_ele, top);
      } else if (lep_id == ParticleID::p_muminus){
        return std::make_pair(FatJetLabel::Top_mu, top);
      } else if (lep_id == ParticleID::p_tauminus){
	for (unsigned kdau=0; kdau < lep->numberOfDaughters(); ++kdau){
	  if ( std::abs(lep->daughter(kdau)->pdgId()) == ParticleID::p_eminus ||
	       std::abs(lep->daughter(kdau)->pdgId()) == ParticleID::p_muminus )
	    return std::make_pair(FatJetLabel::Top_tauL, top);
	}
	return std::make_pair(FatJetLabel::Top_tauH, top);
      }
    }
    else {
      return std::make_pair(FatJetLabel::Top_b, top);
    }
  }
  return std::make_pair(FatJetLabel::Top_ISR, top);
}

std::pair<FatJetMatching::FatJetLabel,const reco::GenParticle*> FatJetMatching::w_label(const pat::Jet* jet, const reco::GenParticle *parton, double distR)
{

  auto w = getFinal(parton);
  if (isHadronic(w)) {
    if (debug_){
      using namespace std;
      cout << "jet: " << jet->polarP4() << endl;
      cout << "W:   "; printGenParticleInfo(w, -1);
    }

    auto wdaus = getDaughterQuarks(w);
    if (wdaus.size() < 2) throw std::logic_error("[FatJetMatching::w_label] W decay has less than 2 quarks!");
    {
      double dr_q1    = reco::deltaR(jet->p4(), wdaus.at(0)->p4());
      double dr_q2    = reco::deltaR(jet->p4(), wdaus.at(1)->p4());
      if (dr_q1 > dr_q2){
        // swap q1 and q2 so that dr_q1<=dr_q2
        std::swap(dr_q1, dr_q2);
        std::swap(wdaus.at(0), wdaus.at(1));
      }
      auto pdgid_q1 = std::abs(wdaus.at(0)->pdgId());
      auto pdgid_q2 = std::abs(wdaus.at(1)->pdgId());

      if (debug_){
        using namespace std;
        cout << "deltaR(jet, q1)    : " << dr_q1 << endl;
        cout << "deltaR(jet, q2)    : " << dr_q2 << endl;
        cout << "pdgid(q1)        : " << pdgid_q1 << endl;
        cout << "pdgid(q2)        : " << pdgid_q2 << endl;
      }

      if (dr_q1<distR && dr_q2<distR){
        if (pdgid_q1 >= ParticleID::p_c || pdgid_q2 >= ParticleID::p_c) {
          return std::make_pair(FatJetLabel::W_cq, w);
        } else {
          return std::make_pair(FatJetLabel::W_qq, w);
        }
      } else if (dr_q1<distR) {
        if (pdgid_q1 >= ParticleID::p_c) {
          return std::make_pair(FatJetLabel::W_c, w);
        } else {
          return std::make_pair(FatJetLabel::W_q, w);
        }
      }
    }
    return std::make_pair(FatJetLabel::W_ISR, w);
  }
  return std::make_pair(FatJetLabel::Invalid, nullptr);
}

std::pair<FatJetMatching::FatJetLabel,const reco::GenParticle*> FatJetMatching::z_label(const pat::Jet* jet, const reco::GenParticle *parton, double distR)
{

  auto z = getFinal(parton);
  if (isHadronic(z)) {
    if (debug_){
      using namespace std;
      cout << "jet: " << jet->polarP4() << endl;
      cout << "Z:   "; printGenParticleInfo(z, -1);
    }

    auto zdaus = getDaughterQuarks(z);
    if (zdaus.size() < 2) throw std::logic_error("[FatJetMatching::z_label] Z decay has less than 2 quarks!");
    {
      double dr_q1    = reco::deltaR(jet->p4(), zdaus.at(0)->p4());
      double dr_q2    = reco::deltaR(jet->p4(), zdaus.at(1)->p4());
      if (dr_q1 > dr_q2){
        // swap q1 and q2 so that dr_q1<=dr_q2
        std::swap(dr_q1, dr_q2);
        std::swap(zdaus.at(0), zdaus.at(1));
      }
      auto pdgid_q1 = std::abs(zdaus.at(0)->pdgId());
      auto pdgid_q2 = std::abs(zdaus.at(1)->pdgId());

      if (debug_){
        using namespace std;
        cout << "deltaR(jet, q1)    : " << dr_q1 << endl;
        cout << "deltaR(jet, q2)    : " << dr_q2 << endl;
        cout << "pdgid(q1)        : " << pdgid_q1 << endl;
        cout << "pdgid(q2)        : " << pdgid_q2 << endl;
      }

      if (dr_q1<distR && dr_q2<distR){
        if (pdgid_q1 == ParticleID::p_b && pdgid_q2 == ParticleID::p_b) {
          return std::make_pair(FatJetLabel::Z_bb, z);
        }else if (pdgid_q1 == ParticleID::p_c && pdgid_q2 == ParticleID::p_c) {
          return std::make_pair(FatJetLabel::Z_cc, z);
        }else {
          return std::make_pair(FatJetLabel::Z_qq, z);
        }
      } else if (dr_q1<distR){
        if (pdgid_q1 == ParticleID::p_b) {
          return std::make_pair(FatJetLabel::Z_b, z);
        }else if (pdgid_q1 == ParticleID::p_c) {
          return std::make_pair(FatJetLabel::Z_c, z);
        }else {
          return std::make_pair(FatJetLabel::Z_q, z);
        }
      }
    }
    return std::make_pair(FatJetLabel::Z_ISR, z);
  }
  return std::make_pair(FatJetLabel::Invalid, nullptr);
}

std::pair<FatJetMatching::FatJetLabel,const reco::GenParticle*> FatJetMatching::higgs_label(const pat::Jet* jet, const reco::GenParticle *parton, double distR)
{

  auto higgs = getFinal(parton);

  if (debug_){
    using namespace std;
    cout << "jet: " << jet->polarP4() << endl;
    cout << "H:   "; printGenParticleInfo(higgs, -1);
  }

  bool is_hVV = false;
  if (higgs->numberOfDaughters() >= 3) {
    // e.g., h->Vqq or h->qqqq
    is_hVV = true;
  }else {
    // e.g., h->VV*
    for (const auto &p : higgs->daughterRefVector()){
      auto pdgid = std::abs(p->pdgId());
      if (pdgid == ParticleID::p_Wplus || pdgid == ParticleID::p_Z0){
        is_hVV = true;
        break;
      }
    }
  }

  int num_h_A0 = 0;
  if (higgs->numberOfDaughters() == 2) {
    for (const auto &p : higgs->daughterRefVector()){
      auto pdgid = std::abs(p->pdgId());
      if (pdgid == ParticleID::p_A0){
        num_h_A0 += 1;
      }
    }
  }
  bool is_hAA = (num_h_A0 == 2);

  if (is_hVV){
    // h->WW or h->ZZ
    std::vector<const reco::GenParticle*> hVV_daus;
    for (unsigned idau=0; idau<higgs->numberOfDaughters(); ++idau){
      const auto *dau = dynamic_cast<const reco::GenParticle*>(higgs->daughter(idau));
      auto pdgid = std::abs(higgs->daughter(idau)->pdgId());
      if (pdgid >= ParticleID::p_d && pdgid <= ParticleID::p_b){
        hVV_daus.push_back(dau);
      }else{
        const auto d = getDaughterQuarks(getFinal(dau));
        hVV_daus.insert(hVV_daus.end(), d.begin(), d.end());
      }
    }

    if (debug_){
      using namespace std;
      cout << "Found " << hVV_daus.size() << " quarks from Higgs decay" << endl;
      for (const auto * gp : hVV_daus){
        using namespace std;
        printGenParticleInfo(gp, -1);
        cout << " ... dR(q, jet) = " << reco::deltaR(*gp, *jet) << endl;
      }
    }

    unsigned n_quarks_in_jet = 0;
    for (const auto *gp : hVV_daus){
      auto dr = reco::deltaR(*gp, *jet);
      if (dr < distR){
        ++n_quarks_in_jet;
      }
    }
    if (n_quarks_in_jet >= 4){
      return std::make_pair(FatJetLabel::H_qqqq, higgs);
    }

  }else if (is_hAA){
    std::vector<const reco::GenParticle*> hAA_gdaus;
    for (unsigned idau=0; idau<higgs->numberOfDaughters(); ++idau){
      const auto *dau = dynamic_cast<const reco::GenParticle*>(higgs->daughter(idau));
      for (unsigned jdau=0; jdau<dau->numberOfDaughters(); ++jdau){
	const auto *gdau = dynamic_cast<const reco::GenParticle*>(dau->daughter(jdau));
	auto pdgid = std::abs(gdau->pdgId());
	if (pdgid == ParticleID::p_b){
	  hAA_gdaus.push_back(gdau);
	}
      }
    }

    if (debug_){
      using namespace std;
      cout << "Found " << hAA_gdaus.size() << " bs from Higgs decay" << endl;
      for (const auto * gp : hAA_gdaus){
        using namespace std;
        printGenParticleInfo(gp, -1);
        cout << " ... dR(b, jet) = " << reco::deltaR(*gp, *jet) << endl;
      }
    }

    unsigned n_bs_in_jet = 0;
    for (const auto *gp : hAA_gdaus){
      auto dr = reco::deltaR(*gp, *jet);
      if (dr < distR){
        ++n_bs_in_jet;
      }
    }
    if (n_bs_in_jet == 4){
      return std::make_pair(FatJetLabel::H_aa_bbbb, higgs);
    }else{
      return std::make_pair(FatJetLabel::H_aa_other, higgs);
    }
  }else if (isHadronic(higgs)) {
    // direct h->qq

    auto hdaus = getDaughterQuarks(higgs);
    if (hdaus.size() < 2) throw std::logic_error("[FatJetMatching::higgs_label] Higgs decay has less than 2 quarks!");
//    if (zdaus.size() >= 2)
    {
      double dr_q1    = reco::deltaR(jet->p4(), hdaus.at(0)->p4());
      double dr_q2    = reco::deltaR(jet->p4(), hdaus.at(1)->p4());
      if (dr_q1 > dr_q2){
        // swap q1 and q2 so that dr_q1<=dr_q2
        std::swap(dr_q1, dr_q2);
        std::swap(hdaus.at(0), hdaus.at(1));
      }
      auto pdgid_q1 = std::abs(hdaus.at(0)->pdgId());
      auto pdgid_q2 = std::abs(hdaus.at(1)->pdgId());

      if (debug_){
        using namespace std;
        cout << "deltaR(jet, q1)    : " << dr_q1 << endl;
        cout << "deltaR(jet, q2)    : " << dr_q2 << endl;
        cout << "pdgid(q1)        : " << pdgid_q1 << endl;
        cout << "pdgid(q2)        : " << pdgid_q2 << endl;
      }

      if (dr_q1<distR && dr_q2<distR){
        if (pdgid_q1 == ParticleID::p_b && pdgid_q2 == ParticleID::p_b) {
          return std::make_pair(FatJetLabel::H_bb, higgs);
        }else if (pdgid_q1 == ParticleID::p_c && pdgid_q2 == ParticleID::p_c) {
          return std::make_pair(FatJetLabel::H_cc, higgs);
        }else {
          return std::make_pair(FatJetLabel::H_qq, higgs);
        }
      }
    }
  }else {
    // test h->tautau
    std::vector<const reco::GenParticle*> taus;
    for (unsigned i=0; i<higgs->numberOfDaughters(); ++i){
      const auto *dau = dynamic_cast<const reco::GenParticle*>(higgs->daughter(i));
      if (std::abs(dau->pdgId()) == ParticleID::p_tauminus){
        taus.push_back(dau);
      }
    }
    if (taus.size() == 2){
      // higgs -> tautau
      // use first version or last version of the tau in dr?
      double dr_tau1    = reco::deltaR(jet->p4(), taus.at(0)->p4());
      double dr_tau2    = reco::deltaR(jet->p4(), taus.at(1)->p4());

      if (debug_){
        using namespace std;
        cout << "deltaR(jet, tau1)    : " << dr_tau1 << endl;
        cout << "deltaR(jet, tau2)    : " << dr_tau2 << endl;
      }

      auto isHadronicTau = [](const reco::GenParticle* tau){
        for (const auto &dau : tau->daughterRefVector()){
          auto pdgid = std::abs(dau->pdgId());
          if (pdgid==ParticleID::p_eminus || pdgid==ParticleID::p_muminus){
            return false;
          }
        }
        return true;
      };

      auto tau1 = getFinal(taus.at(0));
      auto tau2 = getFinal(taus.at(1));
      if (dr_tau1<distR && dr_tau2<distR){
        if (isHadronicTau(tau1) && isHadronicTau(tau2)) {
          return std::make_pair(FatJetLabel::H_tautau, higgs);
        }
      }
    }
  }

  return std::make_pair(FatJetLabel::Invalid, nullptr);

}

std::pair<FatJetMatching::FatJetLabel,const reco::GenParticle*> FatJetMatching::qcd_label(const pat::Jet* jet, const reco::GenParticleCollection& genParticles, double distR)
{

  const reco::GenParticle *parton = nullptr;
  double minDR = 999;
  for (const auto &gp : genParticles){
    if (gp.status() != 23) continue;
    auto pdgid = std::abs(gp.pdgId());
    if (!(pdgid<ParticleID::p_t || pdgid==ParticleID::p_g)) continue;
    auto dr = reco::deltaR(gp, *jet);
    if (dr<distR && dr<minDR){
      minDR = dr;
      parton = &gp;
    }
  }
  if (debug_){
    using namespace std;
    if (parton){
      cout << "parton"; printGenParticleInfo(parton, -1);
      cout << "dr(jet, parton): " << minDR << endl;
    }
  }

  auto n_bHadrons = jet->jetFlavourInfo().getbHadrons().size();
  auto n_cHadrons = jet->jetFlavourInfo().getcHadrons().size();

  if (n_bHadrons>=2) {
    return std::make_pair(FatJetLabel::QCD_bb, parton);
  }else if (n_bHadrons==1){
    return std::make_pair(FatJetLabel::QCD_b, parton);
  }else if (n_cHadrons>=2){
    return std::make_pair(FatJetLabel::QCD_cc, parton);
  }else if (n_cHadrons==1){
    return std::make_pair(FatJetLabel::QCD_c, parton);
  }

  return std::make_pair(FatJetLabel::QCD_others, parton);
}
