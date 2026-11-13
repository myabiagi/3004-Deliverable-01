# DragonEndPOS — Deliverable 1

## Requirements

- Course Ubuntu virtual machine (or OpenStack environment) with Qt Creator 6.0.2 and the provided Qt Widgets development kit
- qmake build system included with the course Qt installation

## Installation and launch in Qt Creator

1. Extract the submission ZIP into a folder on the course VM. Keep the directory structure unchanged.
2. Open Qt Creator. Select "File > Open File or Project", then open `DragonEndCafePOS.pro` from the extracted folder.
3. Select "Build > Build Project" (or press Ctrl+B).
4. Select "Build > Run" (or press Ctrl+R).
5. The Dragon's End Café identification window appears.

## Default account identifiers

Customers:
- `mya.biagi`
- `finnick.stormrider`
- `mira.willowstep`
- `cedric.ashborne`
- `lyra.moonwhisper`

Kitchen staff:
- `simon.siju`
- `rowan.emberpot`

Cashiers:
- `ishaan.bahl`
- `nora.quicktill`

Manager:
- `briar.stonekettle`

Administrator:
- `sage.ledgerkeep`

## Deliverable 1 scope / not implemented

The following full-project features are excluded from the Deliverable 1 prototype:

- External payment authorization (D1 orders are automatically recorded as paid)
- Online order cancellation and refunds
- In-person cashier ordering and pickup transitions
- Inventory quantities, sold-out handling, and ingredient stock deduction
- Manager menu/inventory editing and vendor orders
- Administrator account creation and reports
- Audit logs, session timeout, and database/file persistence
