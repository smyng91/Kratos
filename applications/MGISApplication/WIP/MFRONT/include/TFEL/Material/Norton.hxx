/*!
* \file   TFEL/Material/Norton.hxx
* \brief  this file implements the Norton Behaviour.
*         File generated by tfel version 3.3.0-dev
* \author Helfer Thomas
* \date   23 / 11 / 06
 */

#ifndef LIB_TFELMATERIAL_NORTON_HXX
#define LIB_TFELMATERIAL_NORTON_HXX

#include<string>
#include<iostream>
#include<limits>
#include<stdexcept>
#include<algorithm>

#include"TFEL/Raise.hxx"
#include"TFEL/PhysicalConstants.hxx"
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include"TFEL/TypeTraits/IsReal.hxx"
#include"TFEL/Math/General/IEEE754.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"
#include"TFEL/Material/MechanicalBehaviourTraits.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"
#include"TFEL/Material/BoundsCheck.hxx"
#include"TFEL/Material/IsotropicPlasticity.hxx"
#include"TFEL/Material/Lame.hxx"
#include"TFEL/Material/Hosford1972YieldCriterion.hxx"
#include"TFEL/Material/NortonBehaviourData.hxx"
#include"TFEL/Material/NortonIntegrationData.hxx"

#include"TFEL/Math/General/BaseCast.hxx"

#include "MFront/GenericBehaviour/State.hxx"
#include "MFront/GenericBehaviour/BehaviourData.hxx"
namespace tfel{

namespace material{

struct NortonParametersInitializer
{
static NortonParametersInitializer&
get();

double minimal_time_step_scaling_factor;
double maximal_time_step_scaling_factor;
double theta;
double epsilon;
unsigned short iterMax;

void set(const char* const,const double);

void set(const char* const,const unsigned short);

/*!
 * \brief convert a string to double
 * \param[in] p : parameter
 * \param[in] v : value
 */
static double getDouble(const std::string&,const std::string&);
/*!
 * \brief convert a string to unsigned short
 * \param[in] p : parameter
 * \param[in] v : value
 */
static unsigned short getUnsignedShort(const std::string&,const std::string&);
private :

NortonParametersInitializer();

NortonParametersInitializer(const NortonParametersInitializer&);

NortonParametersInitializer&
operator=(const NortonParametersInitializer&);
/*!
 * \brief read the parameters from the given file
 * \param[out] pi : parameters initializer
 * \param[in]  fn : file name
 */
static void readParameters(NortonParametersInitializer&,const char* const);
};

//! \brief forward declaration
template<ModellingHypothesis::Hypothesis,typename Type,bool use_qt>
class Norton;

//! \brief forward declaration
template<ModellingHypothesis::Hypothesis hypothesis,typename Type>
std::ostream&
 operator <<(std::ostream&,const Norton<hypothesis,Type,false>&);

/*!
* \class Norton
* \brief This class implements the Norton behaviour.
* \param hypothesis, modelling hypothesis.
* \param Type, numerical type.
* \author Helfer Thomas
* \date   23 / 11 / 06
* This viscoplastic behaviour is fully determined by the evolution 
* of the equivalent viscoplastic strain \(p\) as a function \(f\) 
* of the Von Mises stress \(\sigmaeq\) : 
* \[ 
* \dot{p}=f\paren{\sigmaeq}=A\,\sigmaeq^{E} 
* \] 
* where : 
* 
* - \(A\) is the Norton coefficient . 
* - \(E\) is the Norton exponent . 
* 
* \(A\) and \(E\) are declared as material properties . 
*/
template<ModellingHypothesis::Hypothesis hypothesis,typename Type>
class Norton<hypothesis,Type,false> final
: public MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>,
public NortonBehaviourData<hypothesis,Type,false>,
public NortonIntegrationData<hypothesis,Type,false>
{

static constexpr unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;

TFEL_STATIC_ASSERT(N==1||N==2||N==3);
TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<Type>::cond);
TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);

friend std::ostream& operator<< <>(std::ostream&,const Norton&);

static constexpr unsigned short TVectorSize = N;
typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;
static constexpr unsigned short StensorSize = StensorDimeToSize::value;
typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;
static constexpr unsigned short TensorSize = TensorDimeToSize::value;

using ushort =  unsigned short;
using Types = tfel::config::Types<N,Type,false>;
using real                = typename Types::real;
using time                = typename Types::time;
using length              = typename Types::length;
using frequency           = typename Types::frequency;
using stress              = typename Types::stress;
using strain              = typename Types::strain;
using strainrate          = typename Types::strainrate;
using stressrate          = typename Types::stressrate;
using temperature         = typename Types::temperature;
using thermalexpansion    = typename Types::thermalexpansion;
using thermalconductivity = typename Types::thermalconductivity;
using massdensity         = typename Types::massdensity;
using TVector             = typename Types::TVector;
using Stensor             = typename Types::Stensor;
using Stensor4            = typename Types::Stensor4;
using FrequencyStensor    = typename Types::FrequencyStensor;
using ForceTVector        = typename Types::ForceTVector;
using StressStensor       = typename Types::StressStensor;
using StressRateStensor   = typename Types::StressRateStensor;
using DisplacementTVector = typename Types::DisplacementTVector;
using StrainStensor       = typename Types::StrainStensor;
using StrainRateStensor   = typename Types::StrainRateStensor;
using StiffnessTensor     = typename Types::StiffnessTensor;
using Tensor              = typename Types::Tensor;
using FrequencyTensor     = typename Types::FrequencyTensor;
using StressTensor        = typename Types::StressTensor;
using ThermalExpansionCoefficientTensor = typename Types::ThermalExpansionCoefficientTensor;
using DeformationGradientTensor         = typename Types::DeformationGradientTensor;
using DeformationGradientRateTensor     = typename Types::DeformationGradientRateTensor;
using TemperatureGradient = typename Types::TemperatureGradient;
using HeatFlux = typename Types::HeatFlux;
using TangentOperator   = StiffnessTensor;
using PhysicalConstants = tfel::PhysicalConstants<real>;

public :

typedef NortonBehaviourData<hypothesis,Type,false> BehaviourData;
typedef NortonIntegrationData<hypothesis,Type,false> IntegrationData;
typedef typename MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::SMFlag SMFlag;
typedef typename MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::SMType SMType;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::ELASTIC;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::SECANTOPERATOR;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::TANGENTOPERATOR;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::CONSISTENTTANGENTOPERATOR;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::NOSTIFFNESSREQUESTED;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::STANDARDTANGENTOPERATOR;
using IntegrationResult = typename MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::IntegrationResult;

using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::SUCCESS;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::FAILURE;
using MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::UNRELIABLE_RESULTS;

using StressFreeExpansionType = StrainStensor;

private :

typedef typename tfel::math::ComputeBinaryResult<strain,time,tfel::math::OpDiv>::Result DstrainDt;
typedef typename tfel::math::ComputeBinaryResult<DstrainDt,stress,tfel::math::OpDiv>::Result DF_DSEQ_TYPE;



StrainStensor deel;
strain dp;

temperature T_;
DstrainDt f;
DF_DSEQ_TYPE df_dseq;
StressStensor se;
stress seq;
stress seq_e;
StrainStensor n;
stress lambda;
stress mu;
stress lambda_tdt;
stress mu_tdt;

real minimal_time_step_scaling_factor;
real maximal_time_step_scaling_factor;
real theta;
real epsilon;
ushort iterMax;

//! Tangent operator;
TangentOperator Dt;
//! alias to the tangent operator;
TangentOperator& dsig_ddeto;
void computeFlow(){
using namespace std;
using namespace tfel::math;
using namespace tfel::material;
using std::vector;
#line 40 "Norton.mfront"
const real tmp = this->A*pow(this->seq,this->E-1);
#line 41 "Norton.mfront"
this->f       = tmp*this->seq;
#line 42 "Norton.mfront"
this->df_dseq = this->E*tmp;
}

bool NewtonIntegration(){
using namespace std;
using namespace tfel::math;
bool converge=false;
bool inversible=true;
strain newton_f;
strain newton_df;
real newton_epsilon = 100*std::numeric_limits<real>::epsilon();
stress mu_3_theta = 3*(this->theta)*(this->mu);
unsigned int iter=0;
while((converge==false)&&
(iter<(this->iterMax))&&
(inversible==true)){
this->seq = std::max(this->seq_e-mu_3_theta*(this->dp),real(0));
this->computeFlow();
newton_f  = this->dp - (this->f)*(this->dt);
newton_df = 1+mu_3_theta*(this->df_dseq)*(this->dt);
if(std::abs(base_cast(newton_df))>newton_epsilon){
this->dp -= newton_f/newton_df;
iter+=1;
converge = (std::abs(tfel::math::base_cast(newton_f))<(this->epsilon));
} else {
inversible=false;
}
}

if(inversible==false){
return false;
}

if(iter==this->iterMax){
return false;
}

return true;
}

/*!
* \brief Update internal variables at end of integration
*/
void updateIntegrationVariables(){
}

/*!
* \brief Update internal variables at end of integration
*/
void updateStateVariables(){
this->eel += this->deel;
this->p += this->dp;
}

/*!
* \brief Update auxiliary state variables at end of integration
*/
void updateAuxiliaryStateVariables()
{}

//! \brief Default constructor (disabled)
Norton() =delete ;
//! \brief Copy constructor (disabled)
Norton(const Norton&) = delete;
//! \brief Assignement operator (disabled)
Norton& operator = (const Norton&) = delete;

public:

/*!
* \brief Constructor
*/
Norton(const NortonBehaviourData<hypothesis,Type,false>& src1,
const NortonIntegrationData<hypothesis,Type,false>& src2)
: NortonBehaviourData<hypothesis,Type,false>(src1),
NortonIntegrationData<hypothesis,Type,false>(src2),
deel(typename tfel::math::StensorTraits<StrainStensor>::NumType(0)),
dp(strain(0)),
dsig_ddeto(Dt)
{
using namespace std;
using namespace tfel::math;
using std::vector;
this->minimal_time_step_scaling_factor = NortonParametersInitializer::get().minimal_time_step_scaling_factor;
this->maximal_time_step_scaling_factor = NortonParametersInitializer::get().maximal_time_step_scaling_factor;
this->theta = NortonParametersInitializer::get().theta;
this->epsilon = NortonParametersInitializer::get().epsilon;
this->iterMax = NortonParametersInitializer::get().iterMax;
this->lambda=tfel::material::computeLambda(this->young,this->nu);
this->mu=tfel::material::computeMu(this->young,this->nu);
this->lambda_tdt = this->lambda;
this->mu_tdt     = this->mu;
}

/*
 * \brief constructor for the Generic interface
 * \param[in] mgb_d: behaviour data
 */
Norton(const mfront::gb::BehaviourData& mgb_d)
: NortonBehaviourData<hypothesis,Type,false>(mgb_d),
NortonIntegrationData<hypothesis,Type,false>(mgb_d),
deel(typename tfel::math::StensorTraits<StrainStensor>::NumType(0)),
dp(strain(0)),
dsig_ddeto(Dt)
{
using namespace std;
using namespace tfel::math;
using std::vector;
this->minimal_time_step_scaling_factor = NortonParametersInitializer::get().minimal_time_step_scaling_factor;
this->maximal_time_step_scaling_factor = NortonParametersInitializer::get().maximal_time_step_scaling_factor;
this->theta = NortonParametersInitializer::get().theta;
this->epsilon = NortonParametersInitializer::get().epsilon;
this->iterMax = NortonParametersInitializer::get().iterMax;
this->lambda=tfel::material::computeLambda(this->young,this->nu);
this->mu=tfel::material::computeMu(this->young,this->nu);
this->lambda_tdt = this->lambda;
this->mu_tdt     = this->mu;
tfel::fsalgo::copy<StensorSize >::exe(mgb_d.s0.gradients,this->eto.begin());
tfel::fsalgo::transform<StensorSize>::exe(mgb_d.s1.gradients,mgb_d.s0.gradients,this->deto.begin(),std::minus<real>());
tfel::fsalgo::copy<StensorSize >::exe(mgb_d.s0.thermodynamic_forces,this->sig.begin());
}

/*!
 * \ brief initialize the behaviour with user code
 */
void initialize(){
using namespace std;
using namespace tfel::math;
using std::vector;
this->T_ = this->T+(Norton::theta)*(this->dT);

this->se=2*(this->mu)*(tfel::math::deviator(this->eel+(this->theta)*(this->deto)));
this->seq_e = sigmaeq(this->se);
if(this->seq_e>(real(0.01)*(this->young))*std::numeric_limits<stress>::epsilon()){
this->n = 3*(this->se)/(2*this->seq_e);
} else {
this->n = StrainStensor(strain(0));
}
}

/*!
* \brief set the policy for "out of bounds" conditions
*/
void
setOutOfBoundsPolicy(const OutOfBoundsPolicy policy_value){
this->policy = policy_value;
} // end of setOutOfBoundsPolicy

/*!
* \return the modelling hypothesis
*/
constexpr ModellingHypothesis::Hypothesis
getModellingHypothesis() const{
return hypothesis;
} // end of getModellingHypothesis

/*!
* \brief check bounds
*/
void checkBounds() const{
} // end of checkBounds

IntegrationResult
computePredictionOperator(const SMFlag smflag, const SMType smt) override{
using namespace std;
if(smflag!=MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::STANDARDTANGENTOPERATOR){
throw(runtime_error("Norton::computePredictionOperator : invalid tangent operator flag"));
}
if((smt==ELASTIC)||(smt==SECANTOPERATOR)){
Dt = (this->lambda_tdt)*Stensor4::IxI()+2*(this->mu_tdt)*Stensor4::Id();
} else {
string msg("Norton::computePredictionOperator : ");
msg +="unimplemented feature";
throw(runtime_error(msg));
}

return SUCCESS;
}

real getMinimalTimeStepScalingFactor() const override{
  return this->minimal_time_step_scaling_factor;
}

std::pair<bool,real>
computeAPrioriTimeStepScalingFactor(const real current_time_step_scaling_factor) const override{
const auto time_scaling_factor = this->computeAPrioriTimeStepScalingFactorII();
return {time_scaling_factor.first,
        std::min(std::min(std::max(time_scaling_factor.second,
                                   this->minimal_time_step_scaling_factor),
                          this->maximal_time_step_scaling_factor),
                  current_time_step_scaling_factor)};
}

/*!
* \brief Integrate behaviour law over the time step
*/
IntegrationResult
integrate(const SMFlag smflag,const SMType smt) override{
using namespace std;
if(smflag!=MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::STANDARDTANGENTOPERATOR){
throw(runtime_error("invalid tangent operator flag"));
}
if(!this->NewtonIntegration()){
return MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::FAILURE;
}
if(smt!=NOSTIFFNESSREQUESTED){
if(!this->computeConsistentTangentOperator(smt)){
return MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::FAILURE;
}
}
this->deel = this->deto-(this->dp)*(this->n);
this->updateStateVariables();
this->sig  = (this->lambda_tdt)*trace(this->eel)*StrainStensor::Id()+2*(this->mu_tdt)*(this->eel);
this->updateAuxiliaryStateVariables();
return MechanicalBehaviour<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,hypothesis,Type,false>::SUCCESS;
}

std::pair<bool,real>
computeAPosterioriTimeStepScalingFactor(const real current_time_step_scaling_factor) const override{
const auto time_scaling_factor = this->computeAPosterioriTimeStepScalingFactorII();
return {time_scaling_factor.first,
        std::min(std::min(std::max(time_scaling_factor.second,
                                   this->minimal_time_step_scaling_factor),
                          this->maximal_time_step_scaling_factor),
                 current_time_step_scaling_factor)};
}

/*!
* \brief Update the internal energy at end of the time step
* \param[in] Psi_s: internal energy at end of the time step
*/
void computeInternalEnergy(real& Psi_s) const
{
Psi_s=0;
}

/*!
* \brief Update the dissipated energy at end of the time step
* \param[in] Psi_d: dissipated energy at end of the time step
*/
void computeDissipatedEnergy(real& Psi_d) const
{
Psi_d=0;
}

bool computeConsistentTangentOperator(const SMType smt){
using namespace std;
using tfel::material::computeElasticStiffness;
using tfel::math::st2tost2;
if(smt==CONSISTENTTANGENTOPERATOR){
computeElasticStiffness<N,Type>::exe(this->Dt,this->lambda_tdt,this->mu_tdt);
if(this->seq_e>(real(0.01)*(this->young))*std::numeric_limits<stress>::epsilon()){
const real ccto_tmp_1 =  this->dp/this->seq_e;
const auto& M = st2tost2<N,Type>::M();
this->Dt += -4*(this->mu_tdt)*(this->mu)*(this->theta)*(ccto_tmp_1*M-(ccto_tmp_1-this->df_dseq*(this->dt)/(1+3*(this->mu)*(this->theta)*(this->dt)*this->df_dseq))*((this->n)^(this->n)));
}
} else if((smt==ELASTIC)||(smt==SECANTOPERATOR)){
computeElasticStiffness<N,Type>::exe(this->Dt,this->lambda_tdt,this->mu_tdt);
} else {
return false;}
return true;
}

const TangentOperator& getTangentOperator() const{
return this->Dt;
}

void updateExternalStateVariables(){
this->eto  += this->deto;
this->T += this->dT;
}

//!
~Norton()
 override = default;

private:

std::pair<bool,real> computeAPrioriTimeStepScalingFactorII() const{
return {true,this->maximal_time_step_scaling_factor};
}

std::pair<bool,real> computeAPosterioriTimeStepScalingFactorII() const{
return {true,this->maximal_time_step_scaling_factor};
}

//! policy for treating out of bounds conditions
OutOfBoundsPolicy policy = None;
}; // end of Norton class

template<ModellingHypothesis::Hypothesis hypothesis,typename Type>
std::ostream&
operator <<(std::ostream& os,const Norton<hypothesis,Type,false>& b)
{
using namespace std;
os << "eto : " << b.eto << '\n';
os << "deto : " << b.deto << '\n';
os << "sig : " << b.sig << '\n';
os << "dt : " << b.dt << endl;
os << "T : " << b.T << endl;
os << "dT : " << b.dT << endl;
os << "A : " << b.A << '\n';
os << "E : " << b.E << '\n';
os << "young : " << b.young << '\n';
os << "nu : " << b.nu << '\n';
os << "eel : " << b.eel << '\n';
os << "deel : " << b.deel << '\n';
os << "p : " << b.p << '\n';
os << "dp : " << b.dp << '\n';
os << "T : " << b.T << '\n';
os << "dT : " << b.dT << '\n';
os << "T_ : " << b.T_ << '\n';
os << "f : " << b.f << '\n';
os << "df_dseq : " << b.df_dseq << '\n';
os << "se : " << b.se << '\n';
os << "n : " << b.n << '\n';
os << "minimal_time_step_scaling_factor : " << b.minimal_time_step_scaling_factor << '\n';
os << "maximal_time_step_scaling_factor : " << b.maximal_time_step_scaling_factor << '\n';
os << "theta : " << b.theta << '\n';
os << "epsilon : " << b.epsilon << '\n';
os << "iterMax : " << b.iterMax << '\n';
return os;
}

/*!
* Partial specialisation for Norton.
*/
template<ModellingHypothesis::Hypothesis hypothesis,typename Type>
class MechanicalBehaviourTraits<Norton<hypothesis,Type,false> >
{
static constexpr unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;
static constexpr unsigned short TVectorSize = N;
typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;
static constexpr unsigned short StensorSize = StensorDimeToSize::value;
typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;
static constexpr unsigned short TensorSize = TensorDimeToSize::value;
public:
static constexpr bool is_defined = true;
static constexpr bool use_quantities = false;
static constexpr bool hasStressFreeExpansion = false;
static constexpr bool handlesThermalExpansion = false;
static constexpr unsigned short dimension = N;
typedef Type NumType;
static constexpr unsigned short material_properties_nb = 4;
static constexpr unsigned short internal_variables_nb  = 1+StensorSize;
static constexpr unsigned short external_variables_nb  = 1;
static constexpr unsigned short external_variables_nb2 = 0;
static constexpr bool hasConsistentTangentOperator = true;
static constexpr bool isConsistentTangentOperatorSymmetric = true;
static constexpr bool hasPredictionOperator = true;
static constexpr bool hasAPrioriTimeStepScalingFactor = false;
static constexpr bool hasComputeInternalEnergy = false;
static constexpr bool hasComputeDissipatedEnergy = false;
/*!
* \return the name of the class.
*/
static const char* getName(){
return "Norton";
}

};

/*!
* Partial specialisation for Norton.
*/
template<typename Type>
class MechanicalBehaviourTraits<Norton<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,Type,false> >
{
public:
static constexpr bool is_defined = false;
static constexpr bool use_quantities = false;
static constexpr bool hasStressFreeExpansion = false;
static constexpr bool handlesThermalExpansion = false;
static constexpr unsigned short dimension = 0u;
typedef Type NumType;
static constexpr unsigned short material_properties_nb = 0;
static constexpr unsigned short internal_variables_nb  = 0;
static constexpr unsigned short external_variables_nb  = 0;
static constexpr unsigned short external_variables_nb2 = 0;
static constexpr bool hasConsistentTangentOperator = false;
static constexpr bool isConsistentTangentOperatorSymmetric = false;
static constexpr bool hasPredictionOperator = false;
static constexpr bool hasAPrioriTimeStepScalingFactor = false;
static constexpr bool hasComputeInternalEnergy = false;
static constexpr bool hasComputeDissipatedEnergy = false;
/*!
* \return the name of the class.
*/
static const char* getName(){
return "Norton";
}

};

/*!
* Partial specialisation for Norton.
*/
template<typename Type>
class MechanicalBehaviourTraits<Norton<ModellingHypothesis::PLANESTRESS,Type,false> >
{
public:
static constexpr bool is_defined = false;
static constexpr bool use_quantities = false;
static constexpr bool hasStressFreeExpansion = false;
static constexpr bool handlesThermalExpansion = false;
static constexpr unsigned short dimension = 0u;
typedef Type NumType;
static constexpr unsigned short material_properties_nb = 0;
static constexpr unsigned short internal_variables_nb  = 0;
static constexpr unsigned short external_variables_nb  = 0;
static constexpr unsigned short external_variables_nb2 = 0;
static constexpr bool hasConsistentTangentOperator = false;
static constexpr bool isConsistentTangentOperatorSymmetric = false;
static constexpr bool hasPredictionOperator = false;
static constexpr bool hasAPrioriTimeStepScalingFactor = false;
static constexpr bool hasComputeInternalEnergy = false;
static constexpr bool hasComputeDissipatedEnergy = false;
/*!
* \return the name of the class.
*/
static const char* getName(){
return "Norton";
}

};

} // end of namespace material

} // end of namespace tfel

#endif /* LIB_TFELMATERIAL_NORTON_HXX */