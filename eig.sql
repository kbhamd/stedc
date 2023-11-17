.mode col

ATTACH "eig.db" AS eig;


--SELECT * FROM eig.pragma_table_info("kern");


CREATE TABLE summary AS
SELECT
    count(*) AS calls,
    SUM(grd) AS grid,
    substr(TRIM(kernelname,'"'),1,32) AS kernel,
    SUM(FETCH_SIZE)*1024.0/1024/1024 AS RDMB,
    SUM(WRITE_SIZE)*1024.0/1024/1024 AS WRMB,
    SUM(durationNs)*1e-6 AS ms
FROM
eig.KERN
GROUP BY kernel
;

SELECT
    calls,
    grid,
    kernel,
--    PRINTF("%.2e",RDMB) AS RDMB,
--    PRINTF("%.2e",WRMB) AS WRMB,
    PRINTF("%.2e",ms*1e-3) AS sec,
    PRINTF("%7.2f",RDMB/ms) AS RDGBs,
    PRINTF("%7.2f",WRMB/ms) AS WRGBs,
    PRINTF("%7.2f",(RDMB+WRMB)/ms) AS GBs
FROM summary
ORDER BY ms DESC
;

DETACH eig;
