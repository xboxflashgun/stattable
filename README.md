# stattable
internal fast statistics

I need to rewrite sql to C

Average number of games (xuids):
1 hour:  1.1080559437481431   (428462) -   581.215 (select)
1 day:   1.6880020376019533  (8540118) - 00:20.598 (select)
1 week:  2.9082944830763770 (15230935) - 02:26.422 (select)
1 month: 4.8470248240624429 (19649443) - 02:52.012 (COPY from presence_202405)


With countryid/langid: 57372359 out of 203489347

Time to read list of xuid with lang/country - 00:32.082
Time to read this list sorted - 04:38.210
Time to read full list of xuid with COPY - 00:56.079
Time to read list of xuid with lang/country with COPY - 00:42.251


