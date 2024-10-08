#include <hits.hh>

#include <G4Allocator.hh>
#include <G4Circle.hh>
#include <G4Point3D.hh>
#include <G4UnitsTable.hh>
#include <G4VisAttributes.hh>
#include <G4VVisManager.hh>

G4ThreadLocal G4Allocator<SiHit>* sialloc = nullptr;
G4ThreadLocal G4Allocator<CrystalHit>* cralloc = nullptr;

VirtualHit::VirtualHit(const G4ThreeVector& position, G4double arrivalTime) :
    arrivalTime(arrivalTime),
    position(position)
{ }

VirtualHit::~VirtualHit() { }

const G4ThreeVector&
VirtualHit::peekPosition() const {
    return position;
}

G4double VirtualHit::peekArrivalTime() const {
    return arrivalTime;
}

void VirtualHit::Draw()
{
    auto* visMan = G4VVisManager::GetConcreteInstance();
    if (visMan == nullptr) return;

    G4Circle c(peekPosition());
    c.SetDiameter(G4VMarker::screen, 5);

    c.SetFillStyle(G4Circle::filled);
    G4VisAttributes a(G4Color(1, 0, 0));
    c.SetVisAttributes(a);
    visMan->Draw(c);
}

SiHit::SiHit(
    const G4ThreeVector& position,
    G4double arrivalTime,
    G4double depositedEnergy
) :
    VirtualHit(position, arrivalTime),
    depositedEnergy(depositedEnergy)
{ }

SiHit::~SiHit()
{ }

void SiHit::Print()
{
    auto f = G4cout.flags();
    G4cout
        << "=== Begin SiHit ===" << G4endl
        << "Energy deposited: " << std::setw(7) << G4BestUnit(depositedEnergy, "Energy") << G4endl
        << "Position:         " << std::setw(7) << G4BestUnit(position, "Length") << G4endl
        << "Arrival time:     " << std::setw(7) << G4BestUnit(arrivalTime, "Time") << G4endl
        << "=== End SiCrystalHit ===" << G4endl;
    G4cout.flags(f);
    G4cout.flush();
}

bool SiHit::operator==(const SiHit& rhs)
{
    return
        position == rhs.peekPosition() &&
        arrivalTime == rhs.peekArrivalTime() &&
        depositedEnergy == rhs.peekDepositedEnergy();
}

const SiHit& SiHit::operator=(const SiHit& rhs)
{
    position = rhs.peekPosition();
    arrivalTime = rhs.peekArrivalTime();
    depositedEnergy = rhs.peekDepositedEnergy();
    return *this;
}

G4double SiHit::peekDepositedEnergy() const {
    return depositedEnergy;
}

VirtualHit::HitType SiHit::hitType() const {
    return HitType::Si;
}

CrystalHit::CrystalHit(G4double depositedEnergy, const G4ThreeVector& position) :
    // XXX ignore arrival time
    VirtualHit(position, 0),
    depositedEnergy(depositedEnergy)
{}

const CrystalHit& CrystalHit::operator=(const CrystalHit& rhs)
{
    position = rhs.peekPosition();
    arrivalTime = rhs.peekArrivalTime();
    depositedEnergy = rhs.peekDepositedEnergy();
    return *this;
}

CrystalHit::~CrystalHit() {}

VirtualHit::HitType CrystalHit::hitType() const {
    return HitType::Crystal;
}

G4double CrystalHit::peekDepositedEnergy() const {
    return depositedEnergy;
}
