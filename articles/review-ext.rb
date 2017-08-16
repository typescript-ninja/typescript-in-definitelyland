# encoding: utf-8

module ReVIEW
    class MARKDOWNBuilder
        def builder_init_file
            @blank_seen = nil
            @ul_indent = 0
            @chapter.book.image_types = %w(.png .jpg .jpeg .gif .svg)

            # 独自追加分
            @column = 0
        end

        def inline_kw(str)
            "（#{str.gsub(/\*/, '\*')}）"
        end

        def inline_hd_chap(chap, id)
            n = chap.headline_index.number(id)
            if chap.number and @book.config["secnolevel"] >= n.split('.').size
                str = I18n.t("chapter_quote", "#{n} #{compile_inline(chap.headline(id).caption)}")
            else
                str = I18n.t("chapter_quote", compile_inline(chap.headline(id).caption))
            end
            if @book.config["chapterlink"]
                anchor = "h"+n.gsub(/\./, "-")
                %Q(<a href="#{chap.id}#{extname}##{anchor}">#{str}</a>)
            else
                str
            end
        end

        def column_begin(level, label, caption)
            puts %Q[<div class="column">]
      
            @column += 1
            puts '' if level > 1
            a_id = %Q[<a id="column-#{@column}"></a>]
      
            if caption.empty?
                puts a_id unless label.nil?
            else
                if label.nil?
                    puts %Q[<h#{level}>#{a_id}#{compile_inline(caption)}</h#{level}>]
                else
                    puts %Q[<h#{level} id="#{normalize_id(label)}">#{a_id}#{compile_inline(caption)}</h#{level}>]
                end
            end
            # headline(level, label, caption)
        end
      
        def column_end(level)
            puts '</div>'
        end
    end
end
