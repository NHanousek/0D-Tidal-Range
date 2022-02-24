// run the trs for a given period, and determine the output from it
// thus it can be plugged into the BayeOpt or GA or GS of your choice...
// Start by loading basically the whole state of the lagoon and
// load them using their proper names? or is that pointles...
// creating a flex scope.
double flex(double hs, double he, double hpe) {
  double flxT = SimTime;
  double flxUpstream = upStream;
  double flxSluiceQ = sluiceQ;
  double flxInflow = inFlowRate;
  int flxMode = mode;
  double flxProduct = 0; // the variable to be maximised
  double flxMax = flxUpstream;
  double flxMin = flxUpstream; // min and max hold params for range based analysis
  vector<double> flxDownStream = downStream;
  vector<double> flxTurbineQ = turbineQ;
  vector<double> flxHeadDiff = headDiff;
  vector<double> flxPowerOut = powerOut;

  while (flxT < SimTime + flexPeriod) {
    double flxRamp = trsRamp(flxT, flxPrevSwitchTime, rampTime);

    double flxVolumeChange = flxSluiceQ + flxInflow;
    for (int i = 0; i < numBanks; i++) {
      flxVolumeChange += flxTurbineQ[i];
    }
    flxVolumeChange *= -1 * (flexDt * 60 * 60);
    double flxNewUpstream = trsArea.newWaterLevel(flxUpstream, flxVolumeChange);

    if ((flxUpstream > flxDownStream[0] && flxNewUpstream < externalWaterLevel[0].getLevel(flxT)) ||
    (flxUpstream < flxDownStream[0] && flxNewUpstream > externalWaterLevel[0].getLevel(flxT))) {
      flxUpstream = externalWaterLevel[0].getLevel(flxT);
    }
    else {
      flxUpstream = flxNewUpstream;
    }

    // run the model as if it were fixed head.
    bool flxSafetyHold = false;
    bool flxDryUpstream = false;
    for (int i = 0; i < numBanks; i++) {
      flxDownStream[i] = externalWaterLevel[i].getLevel(flxT);
      flxHeadDiff[i] = flxUpstream - flxDownStream[i];

      // do any of the banks trip the safety warning?
      if (flxUpstream <= turbines[i].safeLevel(safetyBuffer) || flxDownStream[i] <= turbines[i].safeLevel(safetyBuffer)) {
        flxSafetyHold = true;
      }
    }
    if (flxUpstream <= trsArea.minInternalLevel + safetyBuffer) {
      flxDryUpstream = true;
    }
    // this is where it gets tricky/interesting.
    // crude initial guess based on first set of banks
    // 'true' tells the nextmode function that we're in flex operation
    // so that it shouldn't disturb the real prevSwitchMode
    flxMode = nextMode(flxHeadDiff[0], flxT, hs, he, hpe, flxMode, true);

    if (flxSafetyHold == true) {
      if (flxMode != 2) {
        flxPrevSwitchTime = flxT;
      }
      flxMode = 2;
    }

    for (int i = 0; i < numBanks; i++) {
      switch (flxMode) {
        case 1: // Filling/sluicing
        if (isParrallelSluice) {
          flxPowerOut[i] = turbines[i].getPower(flxHeadDiff[i]) * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
          flxTurbineQ[i] = turbines[i].getFlow(flxHeadDiff[i]) * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
          //flxSluiceQ = orificeFlow(sluiceCd, flxHeadDiff[0], areaSluices) * flxRamp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
        } else {
          flxPowerOut[i] = 0;
          flxTurbineQ[i] = turbines[i].orifice(flxHeadDiff[i]) * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
          //flxSluiceQ = orificeFlow(sluiceCd, flxHeadDiff[0], areaSluices) * flxRamp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
        }
        if (flxDryUpstream) {
          if (flxTurbineQ[i] > 0) {
            flxTurbineQ[i] = 0;
            //flxSluiceQ = 0;
            flxPowerOut[i] = 0;
          }
        }
        break;
        case 2: // Holding
        flxPowerOut[i] = 0;
        flxTurbineQ[i] = 0;
        //flxSluiceQ = 0;
        break;
        case 3: // Generating
        flxSluiceQ = 0;
        if (flxDryUpstream) {
          if (flxTurbineQ[i] > 0) {
            flxTurbineQ[i] = 0;
            flxPowerOut[i] = 0;
            flxMode = 2; flxPrevSwitchTime = flxT;
          }
        }
        else {
          flxPowerOut[i] = turbines[i].getPower(flxHeadDiff[i]) * flxRamp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
          flxTurbineQ[i] = turbines[i].getFlow(flxHeadDiff[i]) * flxRamp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
        }
        break;
        case 4: // Pumping
        flxSluiceQ = 0;
        if (flxDryUpstream) {
          flxMode = 2; flxPrevSwitchTime = flxT;
          flxPowerOut[i] = 0;
          flxTurbineQ[i] = 0;
        }
        else {
          flxPowerOut[i] = -1 * turbines[i].getPower(flxHeadDiff[i]) * flxRamp * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i));
          flxTurbineQ[i] = absolute(turbines[i].getFlow(flxHeadDiff[i]) * flxRamp * pumpEff * kronecker(i, getCurveNumber(flxHeadDiff[i], flxMode, i))*pumpEff);
          if (flxUpstream >= midTide) {
            flxTurbineQ[i] *= -1.0;
          }
        }
        break;
      }
      if (flxMode == 1) {
        flxSluiceQ = orificeFlow(sluiceCd, flxHeadDiff[0], areaSluices) * flxRamp;
      }
      else {
        flxSluiceQ = 0.0;
      }
      // here it optimises based on the desired parameter
      // if you want to add more, you can copy/use the methods from
      // the existing two, and add them to the if-else tree.
      if (flexVariable == "profit" || flexVariable == "Profit") {
        flxProduct += flxPowerOut[i] * energyValue.getLevel(flxT) * flexDt;
      }
      else if (flexVariable == "energy" || flexVariable == "Energy") {
        flxProduct += flxPowerOut[i] * flexDt;
      }
      else if (flexVariable == "intertidal" || flexVariable == "Intertidal") {
        if (flxUpstream < flxMin) {
          flxMin = flxUpstream;
        }
        else if (flxUpstream > flxMax) {
          flxMax = flxUpstream;
        }
        // it might be faster to put this elsewhere, but it keeps things
        // nice and compact.
        flxProduct = flxMax - flxMin;
      }
    }
    flxInflow = inFlow.getLevel(flxT);
    flxT += flexDt;
  }
  return flxProduct;
}
