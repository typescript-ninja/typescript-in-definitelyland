class Sample {
    hello = (word = "TypeScript") => "Hello, " + word;
    bye: typeof Sample.prototype.hello;
}

var obj = new Sample();
obj.bye = obj.hello;
