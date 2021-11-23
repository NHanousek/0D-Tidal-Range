# run all three schemes for a variety of conditions
# The big bad script that runs many many models
using CSV, DataFrames, Dates, BayesOpt
cd("/Users/nicolashanousek/Documents/0D/AncilliaryServices/model.nosync/")
function is_in(vect, val)
    for vec in vect
        if vec == val
            return true
        end
    end
    return false
end
function makedir(directoryName::String,force=false)
    if is_in(readdir(),directoryName)
        #msg = "directory $(pwd())/$directoryName already exists"
        if force
            rm(directoryName)
            mkdir(directoryName)
            msg = "$msg -> Forced Overwrite"
        end
        #@warn(msg)
    else
        mkdir(directoryName)
    end
end

struct TRS
    code::String        # Codename (3 letters)
    name::String        # Full name
    prevHDS::Float64    # Fixed start head difference from prior study
    prevHDE::Float64    # Fixed end head difference from prior study
    prevPME::Float64    # Fixed pump end head difference
    prevOp::String
    # prevEOut::Float64   # Annual output at this fixed head diff from lit
    nTurb::Int64        # Number of turbines
    dTurb::Float64      # Diameter of turbine (m)
    aSluice::Float64    # Total Area of sluice
    wlFile::String      # Water level input file
end

struct trsTime
    name::String
    duration::Float64
    timestep::Float64
    flexible::Bool
    flexInterval::Float64
    flexPeriod::Float64
    flexDt:: Float64
    Bayes::Bool
    Params::String
    Base::Bool
end

#Mersey based on the Peel/NWDA params in Aggidis & Benzon 2013...             Annual Energy   OG    Ag+Bnz
MTB = TRS("MTB","Mersey Barrage",3.9,1.2,0.0,"EBB",28,8.0,2592.0,"MTB-Long-2018.ts1")       # 900    1088
WSL = TRS("WSL", "West Somerset Lagoon",4.9,2.5,0.0,"EBB",125,7.2,20000,"WSL-Long-2018.ts1")# 6233.64 na JX Applied Energy 2021
IOL = TRS("IOL","Idealised Offshore",4.4,1.8,1.0,"EBB",100,8.0,10000,"IOL-Long-2018.ts1") # New here...

schemes = [WSL, MTB]#, IOL]

flxNames = Dict(
    "BSF" => "Base Fixed",
    # Each of these for pump & non-pump X Energy & Power
    "BYF" => "Bayes Optimal Fixed",
    "ET" => "Every Tide",
    "EH" => "Every Half",
    "ETN" => "Every Tide Next",
    "EHN" => "Every Half Next",
    "ETN5" => "Every Tide 1.5",
    "EHN5" => "Every Half 1.5",
    "EHO" => "Every Half Optimal",
    "ETO" => "Every Half Optimal",
    )

df = DataFrame( #Store all the relevant data on the model run in one big nasty dataframe
    Scheme = String[],Test = String[],
    TMax = Float64[],Flex = Bool[],
    flexInt = Float64[], flxPer = Float64[],
    BayesOpt = Bool[], BayesPar = String[],
    Operation = String[], pump = String[],
    ParrSluice = Bool[], Target = String[],
    Energy = Float64[], Profit = Float64[],
    Notes = String[]
    )
# re-build the functions depending on the scheme and tests
for schm in schemes
    cd(schm.code)
    makedir.(["inputs", "results", "Bayes", "runs", "flex"])
    #Write the operational/test conditions
    BSF = trsTime("BSF",1440,0.08333,false,0.0,0.0,0.0,false,"nan",true) # non-flexible BASE CASE
    BYF = trsTime("BYF",720,0.08333,false,0.0,0.0,0.0,true,"HSE",false) # Optimise fixed head operation
    BYFL = trsTime("BYFL",1440,0.08333,false,0.0,0.0,0.0,true,"HSE",false) # Optimise fixed head operation
    BYFXL = trsTime("BYFXL",2880,0.08333,false,0.0,0.0,0.0,true,"HSE",false) # Optimise fixed head operation
    ET  = trsTime("ET",1440,0.08333,true,12.4,12.4,0.15,false,"nan",false) # Run a simple ET flex system, try to match prior flex schemes?
    EH  = trsTime("EH",1440,0.08333,true,6.2,6.2,0.15,false,"nan",false) # Simple Every Half tide test
    ETN  = trsTime("ETN",1440,0.08333,true,12.4,24.8,0.15,false,"nan",false) # Run a simple ETN flex system
    EHN  = trsTime("EHN",1440,0.08333,true,6.2,12.4,0.15,false,"nan",false) # Simple Every Half Next tide test
    ETN5  = trsTime("ETN5",1440,0.08333,true,12.4,18.6,0.15,false,"nan",false) # Run a simple ET 1.5 flex system
    EHN5  = trsTime("EHN5",1440,0.08333,true,6.2,9.3,0.15,false,"nan",false) # Simple Every Half 1.5 tide test
    ETO  = trsTime("ETO",1440,0.08333,true,12.4,0.0,0.15,true,"FLX",false) # optimised period ET flex system
    EHO  = trsTime("EHO",1440,0.08333,true,6.2,0.0,0.15,true,"FLX",false) # optimised period Every Half tide test
    flexes = [BSF, BYF, BYFL, BYFXL, ET, EH, ETN, EHN, ETN5, EHN5, ETO, EHO]

    # BSF = trsTime("BSF",72,0.08333,false,0.0,0.0,0.0,false,"nan",true) # non-flexible BASE CASE
    # BYF = trsTime("BYF",72,0.08333,false,0.0,0.0,0.0,true,"HSE",false) # Optimise fixed head operation
    # BYFL = trsTime("BYFL",72,0.08333,false,0.0,0.0,0.0,true,"HSE",false) # Optimise fixed head operation
    # BYFXL = trsTime("BYFXL",72,0.08333,false,0.0,0.0,0.0,true,"HSE",false) # Optimise fixed head operation
    # ET  = trsTime("ET",72,0.08333,true,12.4,12.4,0.15,false,"nan",false) # Run a simple ET flex system, try to match prior flex schemes?
    # EH  = trsTime("EH",72,0.08333,true,6.2,6.2,0.15,false,"nan",false) # Simple Every Half tide test
    # ETN  = trsTime("ETN",72,0.08333,true,12.4,24.8,0.15,false,"nan",false) # Run a simple ETN flex system
    # EHN  = trsTime("EHN",72,0.08333,true,6.2,12.4,0.15,false,"nan",false) # Simple Every Half Next tide test
    # ETN5  = trsTime("ETN5",72,0.08333,true,12.4,18.6,0.15,false,"nan",false) # Run a simple ET 1.5 flex system
    # EHN5  = trsTime("EHN5",72,0.08333,true,6.2,9.3,0.15,false,"nan",false) # Simple Every Half 1.5 tide test
    # ETO  = trsTime("ETO",72,0.08333,true,12.4,0.0,0.15,true,"FLX",false) # optimised period ET flex system
    # EHO  = trsTime("EHO",1440,0.08333,true,6.2,0.0,0.15,true,"FLX",false) # optimised period Every Half tide test
    # flexes = [BSF, BYF, EHN5, ETO]

    for flex in flexes
        # By default test both the ebb and 2 way modes
        operations = ["EBB" "2WY"]
        # if it's the base case test use the prior test case for the scheme
        if flex.Base
            operations = [schm.prevOp]
        end

        for op in operations
            #No pumping, constant pumping, flexible pumping
            pumps = ["NP", "CP", "FP"]
            if flex.Base
                if schm.prevPME == 0.0
                    pumps = ["NP"]
                else
                    pumps = ["CP"]
                end
            elseif flex.flexible
                pumps = ["NP", "FP"]
            end

            for pmp in pumps
                # Parrallel sluicing and serial sluicing
                parSluice = Dict("PS" => true,"SS" => false)

                for sps in ["PS" "SS"]
                    # Are we optimising operation for energy or profit
                    targs = ["EN", "PR"]
                    if flex.Base
                        targs = ["EN"]
                    end
                    for targ in targs
                        modelName = "$(schm.code)-$(flex.name)-$op-$pmp-$sps-$targ"

                        # build the model run function
                        function zdRun(param,optimiser=true)
                            B_fname = "$modelName-BC.dat"
                            schm_name = "$modelName.dat"
                            bcf = open(B_fname,"w")
                            println(bcf,
                                "Config file generated by Run-All0D.jl on $(today())\n",
                                "modelName: $modelName \n",
                                "schemeFile: $(schm_name) \n",
                                "numSchemes: 1 \n",
                                "printFull: true \n",
                                "energyValue: SSP-2018.dat \n",
                                "maxTime(hrs): $(flex.duration) \n",
                                "timeStep(min): $(flex.timestep*60.0) \n",
                                "End"
                                )
                            close(bcf)

                            flxString = String
                            if flex.flexible
                                flxString = string(
                                    "flexible: true \n",
                                    "flexDt(hrs): $(flex.flexDt) \n",
                                    "headDiffStart(m): 1.5 8.5 \n",
                                    "headDiffEnd(m): 1.0 5.0 \n",
                                    "headDiffStartDelta(m): 0.125 \n",
                                    "headDiffEndDelta(m): 0.125 \n",
                                    "flexPeriod(hrs): $(flex.flexPeriod) \n",
                                    "flexInterval(hrs): $(flex.flexInterval) \n",
                                    )
                            else
                                opStr = String
                                if flex.Base
                                    opStr = string(
                                        "operation: $(schm.prevOp) \n",
                                        "headDiffStart(m): $(schm.prevHDS) \n",
                                        "headDiffEnd(m): $(schm.prevHDE) \n",
                                        )
                                else
                                    opStr = string(
                                        "operation: $op \n",
                                        "headDiffStart(m): $(param[1]) \n",
                                        "headDiffEnd(m): $(param[2]) \n",
                                        )
                                end
                                flxString = string(
                                    "flexible: false \n",
                                    opStr
                                )
                            end

                            pmpString = String
                            if pmp == "CP" # Constant pumping
                                pmpEnd = String
                                if flex.Base
                                    pmpEnd = "pumpEnd: $(schm.prevPME) \n"
                                else
                                    pmpEnd = "pumpEnd: $(param[3]) \n"
                                end
                                pmpString = string(
                                    "pumping: true \n",
                                    "pumpControl: head \n",
                                    pmpEnd,
                                    )
                            elseif pmp == "FP" # Flexible pumping
                                pmpString = string(
                                    "pumping: true \n",
                                    "flexPumping: true \n",
                                    "pumpControl: head \n",
                                    "pumpEndMin: 0.0 \n",
                                    "pumpEndMax: 2.5 \n",
                                    "pumpEndDelta: 0.125 \n",
                                    )
                            else # No pumping
                                pmpString = "pumping: false \n"
                            end

                            schmf = open("$(schm_name)","w")
                            println(schmf,
                                "$modelName generated by Run-All0D.jl on $(today())\n",
                                "schemeName: $modelName \n",
                                "numBanks: 1 \n",
                                "turbineFile: HQPTurbinesNew.dat \n",
                                "energyValue: SSP-2018.dat \n",
                                "turbineCentreLevel(m): -100 \n",
                                "schemeArea: Scheme_Area.out \n",
                                "inFlowFile: false \n",
                                "midTideLevel(mD): 0.0 \n",
                                "switchModeBuffer(hrs): 0.25 \n",
                                "rampTime(hrs): 0.25 \n",
                                "parrallelSluice: $(parSluice[sps]) \n",
                                flxString,
                                pmpString,
                                "externalWaterLevel: $(schm.wlFile) \n",
                                "numberTurbines(int): $(schm.nTurb) \n",
                                "turbineDiameter(m): $(schm.dTurb) \n",
                                "areaSluices(m2): $(schm.aSluice) \n",
                                "waterLevelMax(m): 10 \n",
                                "waterLevelMin(m): -10 \n",
                                "pumpEfficiency(0-1): 0.75 \n",
                                "End\n"
                                )
                            close(schmf)
                            run(pipeline(`./../BarraCUDA.out $(B_fname)`,
                                stdout="$(modelName)_out.txt", stderr="$(modelName)_err.txt"))
                            dfm = CSV.File("$(modelName)_results.csv") |> DataFrame
                            energy = dfm[end,"energy[MWh]"]
                            profit = dfm[end,"value[Currency]"]
                            # energy = rand()
                            # profit = rand()
                            targs = Dict("EN" => energy, "PR" => profit)
                            if !optimiser
                                notes = "$(flex.name)"
                                for p in param
                                    notes = "$(notes) $p"
                                end
                                push!(df,
                                    (schm.code,flex.name,flex.duration,
                                    flex.flexible,flex.flexInterval,flex.flexPeriod,
                                    flex.Bayes, flex.Params,op, pmp,parSluice[sps],
                                    targ,energy,profit,notes))
                                CSV.write("Summary-Data-tmp.csv",df)
                            end
                            mv("$(modelName)_results.csv", "results/$(modelName)_results.csv", force=true)
                            mv(schm_name, "inputs/$(schm_name)", force=true)
                            mv(B_fname, "inputs/$(B_fname)", force=true)
                            mv("$(modelName)_out.txt", "runs/$(modelName)_out.txt", force=true)
                            mv("$(modelName)_err.txt", "runs/$(modelName)_err.txt", force=true)
                            if flex.flexible
                                mv("$(modelName)_flx_Info.csv", "flex/$(modelName)_flx_Info.csv", force=true)
                            end
                            return -1.0*targs[targ]
                        end

                        #run the model...
                        if flex.Bayes
                            # run a bayesian optimisation on the parameters as desired...
                            optimizer = Float64[]
                            uppper = Float64[]
                            lower = Float64[]

                            config = ConfigParameters()         # calls initialize_parameters_to_default of the C API
                            set_kernel!(config, "kMaternARD5")  # calls set_kernel of the C API
                            set_save_file!(config, "$(modelName)-BayesOpt.dat")
                            #set_surrogate!(config,"sStudentTProcessJef")
                            config.load_save_flag = 2;
                            config.n_iter_relearn = 12;
                            config.force_jump = 5
                            config.sc_type = SC_MTL
                            config.n_iterations = 50
                            config.verbose_level = 1
                            set_log_file!(config,"Log.dat")

                            if flex.Params == "HSE"
                                if pmp == "NP" # no pumping
                                    upper = [8.5 3.5] # head diff: start end
                                    lower = [4.0 0.0] # head diff: start end
                                else
                                    upper = [8.5 3.5 3.0] # head diff: start end pump
                                    lower = [4.0 0.0 0.0] # head diff: start end pump
                                end
                            elseif flex.Params == "FLX"
                                upper = [flex.flexInterval*2.5]
                                lower = [flex.flexInterval*0.5]
                            else
                                error("Invalid Bayesian optimisation setup")
                            end
                            optimizer, optimum = bayes_optimization(zdRun,lower,upper,config)
                            println(modelName)
                            output = zdRun(optimizer,false)
                            mv("$(modelName)-BayesOpt.dat", "Bayes/$(modelName)-BayesOpt.dat",force=true)
                        else
                            println(modelName)
                            output = zdRun([],false)
                        end
                    end
                end
            end
        end
    end
    cd("..")
end


CSV.write("Summary-Data-final.csv",df)

println("All done!")
