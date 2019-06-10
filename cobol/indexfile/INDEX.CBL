       IDENTIFICATION      DIVISION.
       PROGRAM-ID.         INDEX.
       ENVIRONMENT         DIVISION.
       CONFIGURATION       SECTION.
       INPUT-OUTPUT        SECTION.
       FILE-CONTROL.
           SELECT  TEST01-FILE     ASSIGN  "DATA01"
                                   ORGANIZATION    IS  INDEXED
                                   ACCESS  MODE    IS  DYNAMIC
                                   RECORD  KEY     IS  TEST01-KEY
                                   FILE    STATUS  IS  STS-TEST01.
           SELECT  TEST02-FILE     ASSIGN  "DATA02"
                                   ORGANIZATION    IS  INDEXED
                                   ACCESS  MODE    IS  DYNAMIC
                                   RECORD  KEY     IS  TEST02-KEY
                                   FILE    STATUS  IS  STS-TEST02.
       DATA                DIVISION.
       FILE                SECTION.
       FD  TEST01-FILE.
       01  TEST01-REC.
           COPY    "TEST01.INC".
       FD  TEST02-FILE.
       01  TEST02-REC.
           COPY    "TEST01.INC"   REPLACING    //TEST01//
                                  BY           //TEST02//.
       WORKING-STORAGE SECTION.
       01  STA-AREA.
           03  STS-TEST01        PIC X(02).
           03  STS-TEST02        PIC X(02).
       01  CNT1                  PIC 9(4)  VALUE 0.
       01  CNT2                  PIC 9(4)  VALUE 0.
      **************************************************************************
       PROCEDURE           DIVISION.
       000-MAIN            SECTION.
           PERFORM   100   TIMES
               OPEN    OUTPUT          TEST01-FILE
                             

               ADD 1 TO  CNT1
           END-PERFORM
           EXIT    PROGRAM.
