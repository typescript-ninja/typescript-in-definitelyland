interface Config {
	filePath?: string | null;
	verbose?: boolean;
}

// Configのundefinedとnull無し版
interface ConfigFixed {
	filePath: string;
	verbose: boolean;
}

let config: Config = {
	verbose: true,
};
// filledの型は {} & ConfigFixed & Config
// assignの定義が引数4つまではintersection typesで定義されているため
// assign<T, U, V>(target: T, source1: U, source2: V): T & U & V; が実際の定義
let defaultConfig: ConfigFixed = { filePath: "settings.json", verbose: false };
let filled = Object.assign({}, defaultConfig, config);

// ConfigとConfigFixedには直接の互換性はない！
// error TS2322: Type 'Config' is not assignable to type 'ConfigFixed'.
//   Types of property 'filePath' are incompatible.
//     Type 'string | undefined' is not assignable to type 'string'.
//       Type 'undefined' is not assignable to type 'string'.
// let fixed: ConfigFixed = config;

// filledはfilePathとverboseが存在することが確定しているのでConfigFixedと互換性がある！
let fixed: ConfigFixed = filled;
console.log(fixed);

export { ConfigFixed, fixed }
