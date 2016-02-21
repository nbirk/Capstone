<?php

$postedData = json_decode(file_get_contents("php://input"));
get_filteredGames($postedData->filter);

function get_filteredGames($data)
{
	// The connection string is loooooooong. It's easiest to copy/paste this line. Remember to replace 'username' and 'password'!
$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');

if($data === 'all')
{
	$results = array();
	
	$query = 'select * from Game';
	$stid = oci_parse($conn,$query);
	oci_bind_by_name($stid, ':data', $data);
	oci_execute($stid);
	
	//iterate through each row
	while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
	{
		$results[] = $row;
	}
	
	echo json_encode($results); 
	oci_free_statement($stid);
	oci_close($conn);
}
else{
	$results = array();
	
	$data = $data.'%';
	$query = 'select * from Game where gameName like :data';
	$stid = oci_parse($conn,$query);
	oci_bind_by_name($stid, ':data', $data);
	oci_execute($stid);
	
	//iterate through each row
	while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
	{
		$results[] = $row;
	}
	
	echo json_encode($results); 
	oci_free_statement($stid);
	oci_close($conn);
}

}


?>