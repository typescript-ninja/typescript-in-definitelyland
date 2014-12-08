var array: {}[] | Date;

if (array instanceof Array) {
	// error TS2339: Property 'length' does not exist on type 'Date | {}[]'.
	array.length;
}
