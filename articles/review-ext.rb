# encoding: utf-8

module ReVIEW
  class HTMLBuilder
    def list_body(id, lines, lang)
      id ||= ''
      classNames = ["list"]
      lang ||= File.extname(id).gsub(/\./, '')
      if lang != ''
        classNames.push("language-#{lang}")
      end
      print %Q[<pre class="#{classNames.join(" ")}">]
      body = lines.inject(''){|i, j| i + detab(j) + "\n"}
      lexer = lang || File.extname(id).gsub(/\./, '')
      puts highlight(:body => body, :lexer => lexer, :format => 'html')
      puts '</pre>'
    end
  end
end
