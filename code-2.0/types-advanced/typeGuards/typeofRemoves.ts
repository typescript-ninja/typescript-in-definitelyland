let obj: number | string | boolean = null as any;

if (typeof obj === "string") {
	// ここではstringと確定されている！
	obj.charAt(0);
} else {
	// ここではstringが引かれ number | boolean;
	obj;
}

if (typeof obj === "string") {
	// ここではstringと確定されている！
	obj.charAt(0);
} else if (typeof obj === "number") {
	// ここではnumberと確定されている！
	obj.toFixed(2);
} else {
	// ここではstring, numberが引かれbooleanとなる！
	obj;
}

export {}
