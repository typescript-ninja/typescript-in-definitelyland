function concat<T1, T2>(v1: T1[], v2: T2[]): (T1 | T2)[] {
  var result: (T1 | T2)[] = [];
  v1.forEach(v => result.push(v));
  v2.forEach(v => result.push(v));
  return result;
};

var array = concat([1, "str"], [true]);
// array の型は (string | number | boolean)[] となる
// Argument of type 'Object' is not assignable to parameter of type 'string | number | boolean'.
// array.push(new Object());
