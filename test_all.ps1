$Content=(Get-ChildItem ..\input\).Name ;
$OutPath="..\output\";
for ($i=0; $i -lt $( $Content.length - 1 ) ; $i++){
	$inFile="..\input\"+$Content[$i] ;

	./rotjacobi.exe $inFile $OutPath ;
}
Write-Host "Congratulation ! It is all done ;)."
