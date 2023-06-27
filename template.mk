$(FILENAME).o: $(FILENAME).c $(FILENAME).h
-include $(FILENAME).d

clean:
	$(RM) $(FILENAME).o
	$(RM) $(FILENAME).d
