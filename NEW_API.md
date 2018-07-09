*This file contains notes about the migration to the new API.*

## Things to remember to do

- Create a new `test_common` program that combines `test_rand`, `test_aes`, `test_sha`, etc.
- `speed_kem` needs autoconf to generate compilation information variables
- Frodo ahsn't been configured to use AVX2 where available
