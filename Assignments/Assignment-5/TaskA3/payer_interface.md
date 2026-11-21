# Task A3: Abstract Payer Interface
- `Payer` declares `virtual double amountCovered(const Bill&) const = 0;`.
- Subclasses: `SelfPay`, `Insurance`, `GovernmentScheme`, plus extensible `CorporateTieUp`.
