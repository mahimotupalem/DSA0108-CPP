# Task A1: Polymorphic Bill Hierarchy Design
- Abstract base class `Bill` with `virtual double total() const = 0;` and `virtual ~Bill() = default;`.
- Derived classes: `OutPatientBill`, `InPatientBill`, `DayCareBill`, `EmergencyBill`.
