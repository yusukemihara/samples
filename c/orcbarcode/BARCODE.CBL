       IDENTIFICATION      DIVISION.
       PROGRAM-ID.         BARCODE.
       ENVIRONMENT         DIVISION.
       CONFIGURATION           SECTION.
       DATA                DIVISION.
       WORKING-STORAGE         SECTION.
         COPY    "BARCODE.INC".
       LINKAGE                 SECTION.
      ************************************************************************
       PROCEDURE           DIVISION.
       000-MAIN                SECTION.
           INITIALIZE BC-SET.
           MOVE '12345' TO BC-DATA.
           MOVE '/tmp/out.png' TO FNAME.
           MOVE '1' TO BC-MARGIN.
           MOVE '1' TO BC-SIZE.
           MOVE '64' TO BC-HEIGHT.
           CALL  'orcbarcode' USING
             BC-SET.
           DISPLAY BC-RC.
