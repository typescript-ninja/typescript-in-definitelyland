var tuple: { 0: number; 1: string; 3: boolean; } = <any>[1, "str", true];

tuple[1].charAt(0); // string は charAt を持つ！

// だがしかし(型のうえでは)Arrayではないので次は通らない
// tuple.forEach(v => {
//   console.log(v);
// });
