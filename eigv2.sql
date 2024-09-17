.mode col

.import --csv pmc_1/results_eig.csv prof

SELECT * FROM pragma_table_info('prof');

.print ""

CREATE TABLE stats AS
SELECT
--Dispatch_ID,
--GPU_ID,
--Queue_ID,
--PID,
--TID,
CAST(Grid_Size AS INT)                               AS grid,
CAST(Workgroup_Size AS INT)                          AS work,
CAST(LDS_Per_Workgroup AS INT)                       AS lds,
CAST(Scratch_Per_Workitem AS INT)                    AS scratch,
CAST(Arch_VGPR AS INT)                               AS vgpr,
CAST(Accum_VGPR AS INT)                              AS agpr,
CAST(SGPR AS INT)                                    AS sgpr,
CAST(Wave_Size AS INT)                               AS wave,
--MAX(GL2C_EA_WRREQ_64B_sum)                           AS WR_EA,
MAX(FETCH_SIZE)	                             	     AS FETCH,
--MAX(SQ_WAVES)                              	     AS SQ_WAVES,
--MAX(SQ_BUSY_CYCLES)                                  AS SQ_CYCLES,
SUM(CAST(End_Timestamp-Start_Timestamp AS INT))*1e-6 AS ms,
COUNT(*)                                             AS calls,
SUBSTR(Kernel_Name,1,32)                             AS kernel
--Correlation_ID
FROM prof
GROUP BY kernel
ORDER BY ms DESC
;


SELECT
PRINTF('%9d',grid) AS grid,
PRINTF('%4d',work) AS work,
PRINTF('%5d',lds) AS lds,
PRINTF('%5d',scratch) AS scratch,
PRINTF('%5d',vgpr) AS vgpr,
agpr,
sgpr,
wave,
PRINTF('%11.4f',ms) AS ms,
PRINTF('%6d',calls) AS calls,
kernel
FROM
stats
;

SELECT * FROM stats;
