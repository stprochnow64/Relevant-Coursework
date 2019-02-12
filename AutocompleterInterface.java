package autocompleter;

import java.util.ArrayList;

public interface AutocompleterInterface {

    boolean isEmpty();
    void addTerm(String toAdd);
    boolean hasTerm(String query);
    String getSuggestedTerm(String query);
    ArrayList<String> getSortedTerms();

}