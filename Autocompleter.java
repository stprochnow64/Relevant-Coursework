package autocompleter;

import java.util.ArrayList;
import java.util.Arrays;

public class Autocompleter implements AutocompleterInterface {

    // -----------------------------------------------------------
    // Fields
    // -----------------------------------------------------------
    TTNode root;
    
    // -----------------------------------------------------------
    // Constructor
    // -----------------------------------------------------------
    Autocompleter () {
        root = null;
    }
    
    
    // -----------------------------------------------------------
    // Methods
    // -----------------------------------------------------------
    
    public boolean isEmpty () {
        return (root == null);
    }
    
    public void addTerm (String toAdd) {
        toAdd = normalizeTerm(toAdd);
        addTerm(toAdd, root, 0);
    }
    
    public boolean hasTerm (String query) {
        query = normalizeTerm(query);
        return hasTerm(query, root, 0);
    }
    
    public String getSuggestedTerm (String query) {
        query = normalizeTerm(query);
        return getSuggestedTerm(query, root, 0);
    }
    
    public ArrayList<String> getSortedTerms () {
        ArrayList<String> sortedTerms = new ArrayList<String>();
        return getSortedTerms(sortedTerms, "", root);
    }
    
    
    // -----------------------------------------------------------
    // Helper Methods
    // -----------------------------------------------------------
    
    private String normalizeTerm (String s) {
        // Edge case handling: empty Strings illegal
        if (s == null || s.equals("")) {
            throw new IllegalArgumentException();
        }
        return s.trim().toLowerCase();
    }
    
    /*
     * Returns:
     *   int less than 0 if c1 is alphabetically less than c2
     *   0 if c1 is equal to c2
     *   int greater than 0 if c1 is alphabetically greater than c2
     */
    private int compareChars (char c1, char c2) {
        return Character.toLowerCase(c1) - Character.toLowerCase(c2);
    }
    
    // [!] Add your own helper methods here!
    
    private void addTerm(String toAdd, TTNode current, int index){
        if (isEmpty()){
            if (toAdd.length() == 1){
                current = new TTNode(toAdd.charAt(0), true);
                root = current;
                return;
            }
            current = new TTNode(toAdd.charAt(0), false);
            root = current;

        }
        if (compareChars(toAdd.charAt(index), current.letter) < 0){
            if (current.left == null){
                if (index >= toAdd.length() - 1){
                    current.left = new TTNode(toAdd.charAt(index), true);
                    return;
                }
                current.left = new TTNode(toAdd.charAt(index), false);
            }
            addTerm(toAdd, current.left, index);
        }
        else if (compareChars(toAdd.charAt(index), current.letter) == 0){
            if (toAdd.length() == 1 || index >= toAdd.length() - 1) {
                current.wordEnd = true;
                return;
            }
            index++;
            if (current.mid == null){
                if (index >= toAdd.length() - 1){
                    current.mid = new TTNode(toAdd.charAt(index), true);
                    return;
                }
                current.mid = new TTNode(toAdd.charAt(index), false);
                addTerm(toAdd, current.mid,index);
            }
            addTerm(toAdd, current.mid, index);
            
        }

        else {
            if (current.right == null){
                if (index >= toAdd.length() - 1){
                    current.right = new TTNode(toAdd.charAt(index), true);
                    return;
                }
                current.right=new TTNode(toAdd.charAt(index), false);
            }
            addTerm(toAdd, current.right, index);
        }
    }
    
    private boolean hasTerm(String query, TTNode current, int index){
        if (current == null || index == query.length()){
            return false;
        }
        if (compareChars(query.charAt(index), current.letter) == 0){
            if (index == query.length() - 1 && current.wordEnd){
                return true;
            }
            index++;
            return hasTerm(query, current.mid, index);
        }
        
        if (compareChars(query.charAt(index), current.letter) < 0){
            return hasTerm(query, current.left, index);
        }
        
        if (compareChars(query.charAt(index), current.letter) > 0){
            return hasTerm(query, current.right, index);
        }
        return false;
    }
    
    
    private String getSuggestedTerm (String query, TTNode current, int index) {
        if (current == null) {
            return null;
        }
        if (query.charAt(index) == current.letter && index == query.length() - 1){
            if (current.wordEnd){
                return query;
            }
            return endOfString(query, current);
        }
        if (query.charAt(index) == current.letter){
            index++;
            return getSuggestedTerm(query, current.mid, index);
        }
        if (compareChars(query.charAt(index), current.letter) < 0){
            return getSuggestedTerm(query, current.left, index);
        }
        if (compareChars(query.charAt(index), current.letter) > 0){
            return getSuggestedTerm(query, current.right, index);
        }
        return null;
    }
    
    private ArrayList<String> getSortedTerms (ArrayList<String> sortedTerms, String stringStore, TTNode current){
        
        if (isEmpty()) {
            return sortedTerms;
        }

        if (current.left != null){
            getSortedTerms (sortedTerms, stringStore, current.left);
        }  
        
        if (current.mid != null){
            stringStore += current.letter;
            if (current.wordEnd){
                sortedTerms.add(stringStore);
            } 
            getSortedTerms (sortedTerms, stringStore, current.mid);
        }
        
        if (current.mid == null){
            stringStore += current.letter;
            sortedTerms.add(stringStore);
        }
        
        if (current.right != null){
            stringStore = stringStore.substring(0, stringStore.indexOf(current.letter)) + stringStore.substring(stringStore.indexOf(current.letter) + 1);
            getSortedTerms (sortedTerms, stringStore, current.right);
        }
        return sortedTerms;
    }
    
    
    private String endOfString(String partial, TTNode current) {
        partial = normalizeTerm(partial);
        if (current.mid == null || current.wordEnd) {
            return partial;
        }
        partial += current.mid.letter;
        return endOfString (partial, current.mid);
    }
    
    
    // -----------------------------------------------------------
    // TTNode Internal Storage
    // -----------------------------------------------------------
    
    /*
     * Internal storage of autocompleter search terms
     * as represented using a Ternary Tree with TTNodes
     */
    private class TTNode {
        
        boolean wordEnd;
        char letter;
        TTNode left, mid, right;
        
        TTNode (char c, boolean w) {
            letter  = c;
            wordEnd = w;
            left    = null;
            mid     = null;
            right   = null;
        }
    }  
}