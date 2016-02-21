<?php
$postedData = json_decode(file_get_contents("php://input"));

get_list($postedData->list_id);

function get_list($data)
{
	 
	$results = array();
	$games = array();
	$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');

	//Select customer with last name from field
	$userQuery = 'select userName from Account where listId = :data';
	$listQuery = 'select * from ListGame, Game where listId = :data and ListGame.gameId = Game.gameId';
	$stid = oci_parse($conn,$userQuery);
	$stid2 = oci_parse($conn,$listQuery);
	oci_bind_by_name($stid, ':data', $data);
	oci_bind_by_name($stid2, ':data', $data);
	oci_execute($stid,OCI_DEFAULT);
	
	//iterate through each row
	while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
	{
		$results[] = $row;
	}

	oci_execute($stid2,OCI_DEFAULT);
	while ($row = oci_fetch_array($stid2,OCI_ASSOC)) 
	{
		$games[] = $row;
	}
	$results[] = $games;
	
	echo json_encode($results); 
	oci_free_statement($stid);
	oci_free_statement($stid2);
	oci_close($conn);

	
}
?>