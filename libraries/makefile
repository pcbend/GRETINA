SUBDIRS = TGRUTint TGEBFormat G2Format PWFormat TGretina TPhosWall TKinematics TNucleus GROOT 
ALLDIRS = $(SUBDIRS)

.PHONY: subdirs $(ALLDIRS) clean
subdirs: $(SUBDIRS)

$(ALLDIRS):
	@$(MAKE) -C $@

clean:
	@$(RM) *.a *so *~ .nsf*
	@for dir in $(ALLDIRS); do \
		$(MAKE) -C $$dir $@; \
	done


