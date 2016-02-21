<?php


	$results = array();
	$conn = oci_connect('malz', '1Qaz2wsx', '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=db1.chpc.ndsu.nodak.edu)(Port=1521)))(CONNECT_DATA=(SID=cs)))');

	//Select customer with last name from field
	$query = 'select * from Platform';

	$stid = oci_parse($conn,$query);
	oci_bind_by_name($stid, ':data', $data);
	oci_execute($stid,OCI_DEFAULT);
	
	//iterate through each row
	while ($row = oci_fetch_array($stid,OCI_ASSOC)) 
	{
		$results[] = $row;
	}

	
	echo json_encode($results);
	
	oci_free_statement($stid);
	oci_close($conn);


?>