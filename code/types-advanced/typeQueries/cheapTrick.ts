class Sample {
	hello = (word = "TypeScript") => "Hello, " + word;
	bye: typeof Sample.prototype.hello;
}

let obj = new Sample();
obj.bye = obj.hello;

export { }
