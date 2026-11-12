# DragonEndPOS D1 Model/Controller integration tests

Put the two files in a `tests/` directory in the project root, alongside `model/`, `controller/`, and `DragonEndCafePOS.pro`.

On your Ubuntu VM, run:

```bash
mkdir -p build-tests
cd build-tests
qmake ../tests/d1_tests.pro
make -j2
./d1_tests
```

If `qmake` isn't found, use the Qt kit's qmake path (or try `qmake6` if Qt 6 is installed).

Exit status 0 indicates all checks passed. Failures appear as `FAIL:` lines.

## Default login usernames

Customers:
- `mya.biagi`
- `finnick.stormrider`
- `mira.willowstep`
- `cedric.ashborne`
- `lyra.moonwhisper`

Kitchen staff:
- `ishaan.bahl`
- `rowan.emberpot`

Cashiers:
- `simon.siju`
- `nora.quicktill`

Manager:
- `briar.stonekettle`

Administrator:
- `sage.ledgerkeep`

The test suite checks default records, sessions, pricing, placement, second-order restriction, queue, completion, one-time loyalty points, and persistence in memory across logout/login.

It intentionally does not test UI behavior, payment gateways, in-person order placement, cancellation workflows, inventory, or database persistence.
