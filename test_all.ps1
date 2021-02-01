Param([string]$src, [string]$des);

$Content=Get-ChildItem -Name $src;
$OutPath=$des;
for ($i=0; $i -lt $( $Content.length ) ; $i++){
	$inFile=$src+$Content[$i] ;
	./rotjacobi.exe $inFile $OutPath ;
}
Write-Host "Congratulation ! It is all done ;)."
