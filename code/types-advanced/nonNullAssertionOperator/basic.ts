import * as fs from "fs";

interface Config {
	filePath?: string | null;
	verbose?: boolean;
}

// 呼び出し元で値をしっかり代入していても
let config: Config = {};
config.filePath = "settings.json";
config.verbose = false;
processA(config);
function processA(config: Config = {}) {
	// 関数内部ではConfigのプロパティはundefinedの可能性が排除できない…
	// よって、! で無理やりエラーを消す必要がある
	if (fs.existsSync(config.filePath!)) {
		console.log(fs.readFileSync(config.filePath!, "utf8"));
	} else {
		console.error(`${config.filePath} is not exists`);
	}
}

processB();
function processB(config: Config = {}) {
	// 関数内で初期値を設定してやるとエラーを解消できる（かしこい）
	config.filePath = config.filePath || "settings.json";
	config.verbose = config.verbose || false;

	// 初期値設定済なので ! 不要
	if (fs.existsSync(config.filePath)) {
		console.log(fs.readFileSync(config.filePath, "utf8"));
	} else {
		console.error(`${config.filePath} is not exists`);
	}

	// undefinedではなくした結果は関数をまたいで引き継がれない
	// 残念だが当然…
	processA(config);
}

// ? 除去版を作ってみる
interface ConfigFixed {
	filePath: string;
	verbose: boolean;
}

processC();
function processC(config: Config = {}) {
	// ? 除去版に値を詰め替える
	const fixed: ConfigFixed = {
		filePath: config.filePath || "settings.json",
		verbose: config.verbose || false,
	};

	if (fs.existsSync(fixed.filePath)) {
		console.log(fs.readFileSync(fixed.filePath, "utf8"));
	} else {
		console.error(`${config.filePath} is not exists`);
	}
}
