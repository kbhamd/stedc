.mode col

--SELECT * FROM pragma_table_info('kern');

DROP TABLE IF EXISTS summary;

CREATE TABLE summary AS
SELECT 
count() AS calls,
SUM(grd) AS grd,
wgr,
SUBSTR(KernelName,1,56) AS kernel,
SUM(DurationNs)*1e-6 AS ms
FROM kern
GROUP BY kernel
ORDER BY ms DESC
;

SELECT
    calls,
    grd,
    wgr,
    TRIM(kernel,'"') AS kernel,
    PRINTF("%10.2f", ms) As ms
FROM summary;

