module.exports = function (grunt) {
	function generateTypeScriptCompileSettings() {
		var srcFileList = grunt.file.expand([
			'code/**/*.ts',
			'!code/**/*-invalid.ts', // コンパイルが通らないコード
			'!code/**/invalid.ts', // コンパイルが通らないコード
			'!code/**/*-invalid.d.ts', // コンパイルが通らないコード
			'!code/**/invalid.d.ts', // コンパイルが通らないコード
			'!code/**/*-1.3.0.ts', // 1.3.0 限定コード
			'!code/**/node_modules/**/*.ts'
		]);
		var tasks = {};
		srcFileList.forEach(function (file) {
			taskName = file.replace(/\./g, "-dot-");
			var task = tasks[taskName] = {
				src: file,
				options: {
					compiler: './typescript/bin/tsc',
					target: 'es6',
					module: 'commonjs',
					noImplicitAny: true,
					sourceMap: false,
					declaration: false
				}
			};
			switch (file) {
				case "notExists/hogehoge.ts":
					task.options.declaration = true;
					break;
				default:
					break;
			}
			switch (file) {
				case "notExists/fugafuga.ts":
					task.options.module = "amd";
					break;
				default:
					break;
			}
		});
		return tasks;
	}

	grunt.initConfig({
		ts: generateTypeScriptCompileSettings(),
		tslint: {
			options: {
				configuration: grunt.file.readJSON("tslint.json")
			},
			files: {
				src: [
					'code/**/*.ts'
				]
			}
		},
		'typescript-formatter': {
			options: {
				configuration: grunt.file.readJSON("tsfmt.json")
			},
			files: {
				src: [
					'code/**/*.ts'
				],
				dest: "code"
			}
		},
		dtsm: {
			client: {
				options: {
					// optional: specify config file
					confog: './dtsm.json'
				}
			}
		},
		less: {
			blog: {
				options: {
					paths: ["articles"]
				},
				files: {
					"articles/style.css": "articles/style.less"
				}
			},
			epub: {
				options: {
					paths: ["articles"]
				},
				files: {
					"articles/epub.css": "articles/review.less"
				}
			}
		},
		copy: {
			blog: {
				files: [
					{src: 'articles/_review-ext.rb', dest: 'articles/review-ext.rb'},
					{src: 'articles/layouts/_layout.html.erb', dest: 'articles/layouts/layout.html.erb'}
				]
			},
			public: {
				expand: true,
				cwd: 'articles/',
				src: [
					'*.html',
					'style.css',
					'images/**'
				],
				dest: 'public/'
			}
		},
		clean: {
			ts: {
				src: [
					'code/**/*.js',
					'code/**/*.js.map'
				]
			},
			review: {
				src: [
					'articles/c87-typescript-pdf/',
					'articles/*.pdf',
					'articles/*.epub',
					'articles/*.html',
					'articles/*.css',
					// grifletがgruntを叩けないので
					'!articles/epub.css',
					// epubとhtmlでカスタムテンプレ利用有無を切り替える
					'articles/layouts/layout.html.erb',
					'articles/review-ext.rb'
				]
			},
			public: {
				src: [
					'public/'
				]
			}
		},
		exec: {
			preprocess: {
				cwd: "./articles",
				cmd: function () {
					var command = "bundle exec review-preproc";
					var files = [
						"articles"
					];
					var exec = command + " -r --tabwidth=2 *.re";
					console.log(exec);
					return exec;
				}
			},
			compile2text: {
				cwd: "./articles",
				cmd: function () {
					return "bundle exec review-compile --all --target=text --footnotetext --stylesheet=style.css";
				}
			},
			compile2html: {
				cwd: "./articles",
				cmd: function () {
					return "bundle exec review-compile --all --target=html --footnotetext --stylesheet=style.css --chapterlink";
				}
			},
			compile2latex: {
				cwd: "./articles",
				cmd: function () {
					return "bundle exec review-compile --all --target=latex --footnotetext --stylesheet=style.css";
				}
			},
			compile2idgxml: {
				cwd: "./articles",
				cmd: function () {
					return "bundle exec review-compile --all --target=idgxml --footnotetext --stylesheet=style.css";
				}
			},
			compile2pdf: {
				cwd: "./articles",
				cmd: function () {
					return "bundle exec review-pdfmaker config.yml";
				}
			},
			compile2epub: {
				cwd: "./articles",
				cmd: function () {
					return "bundle exec review-epubmaker config.yml";
				}
			}
		}
	});

	function generateTask(target, pretask) {
		pretask = pretask || [];
		return ['clean', /* 'typescript-formatter', */ 'ts'].concat(pretask).concat(['exec:preprocess', 'exec:compile2' + target]);
	}

	grunt.registerTask(
		'default',
		"原稿をコンパイルしてPDFファイルにする",
		"pdf");

	grunt.registerTask(
		'text',
		"原稿をコンパイルしてTextファイルにする",
		generateTask("text"));

	grunt.registerTask(
		'html',
		"原稿をコンパイルしてHTMLファイルにする",
		generateTask("html", ['less:blog', 'copy:blog']).concat(['copy:public']));

	grunt.registerTask(
		'idgxml',
		"原稿をコンパイルしてInDesign用XMLファイルにする",
		generateTask("idgxml"));

	grunt.registerTask(
		'pdf',
		"原稿をコンパイルしてpdfファイルにする",
		generateTask("pdf"));

	grunt.registerTask(
		'epub',
		"原稿をコンパイルしてepubファイルにする",
		generateTask("epub", ['less:epub']));

	require('load-grunt-tasks')(grunt);
};
