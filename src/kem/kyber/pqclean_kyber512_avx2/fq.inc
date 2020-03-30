.macro red16 r x=12
vpmulhw         %ymm1,%ymm\r,%ymm\x
vpsraw          $10,%ymm\x,%ymm\x
vpmullw         %ymm0,%ymm\x,%ymm\x
vpsubw          %ymm\x,%ymm\r,%ymm\r
.endm

.macro csubq r x=12
vpsubw		%ymm0,%ymm\r,%ymm\r
vpsraw		$15,%ymm\r,%ymm\x
vpand		%ymm0,%ymm\x,%ymm\x
vpaddw		%ymm\x,%ymm\r,%ymm\r
.endm

.macro caddq r x=12
vpsraw		$15,%ymm\r,%ymm\x
vpand		%ymm0,%ymm\x,%ymm\x
vpaddw		%ymm\x,%ymm\r,%ymm\r
.endm

.macro fqmulprecomp al,ah,b x=12
vpmullw		%ymm\al,%ymm\b,%ymm\x
vpmulhw		%ymm\ah,%ymm\b,%ymm\b
vpmulhw		%ymm0,%ymm\x,%ymm\x
vpsubw		%ymm\x,%ymm\b,%ymm\b
.endm
