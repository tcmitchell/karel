
doc:
	mkdir doc
	mkdir doc/html_mono
	cd ../doc; texi2html -monolithic karel.texi
	mv ../doc/*.html doc/html_mono/
	mkdir doc/html_chapter
	cd ../doc; texi2html -split_chapter karel.texi
	mv ../doc/*.html doc/html_chapter/
	mkdir doc/html_node
	cd ../doc; texi2html -split_node karel.texi
	mv ../doc/*.html doc/html_node/

dist:
	tar zcvf karel_web.tar.gz *.html doc

sourceforge:
	scp karel_web.tar.gz karel.sourceforge.net:/home/users/tmitchel/

clean:
	$(RM) -rf doc
