#ifndef BLOCKS_H
#define BLOCKS_H

/* direction A (Předměstí --> Hrad) */
/* direction B (Hrad --> Předměstí) */

/* hall's probes */
#define PROBE_COUNT 4
#define HDA 0 /* hardware: HSS1 */
#define HTA 1 /* hardware: HSR1 */
#define HDB 2 /* hardware: HSS2 */
#define HTB 3 /* hardware: HSR2 */

/* semaphores */
#define SEMAPHORE_COUNT 2
#define SEMA 0 /* hardware: ST1 */
#define SEMB 1 /* hardware: ST2 */

/* junctions */
#define JUNCTION_COUNT 2
#define JUNCTA 0 /* hardware: SV1 */
#define JUNCTB 1 /* hardware: SV2 */

/* paths */
#define PATHS_COUNT 4
#define P_DIRECT_A 0
#define P_DIRECT_B 1
#define P_BAY_A 2
#define P_BAY_B 3

#endif /* BLOCKS_H */
