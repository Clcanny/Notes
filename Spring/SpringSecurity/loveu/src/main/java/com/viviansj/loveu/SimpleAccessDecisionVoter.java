package com.viviansj.loveu;

import org.springframework.security.access.AccessDecisionVoter;
import org.springframework.security.access.ConfigAttribute;
import org.springframework.security.core.Authentication;

import java.util.Collection;

public class SimpleAccessDecisionVoter implements AccessDecisionVoter {

    int ACCESS_GRANTED = 1;
    int ACCESS_ABSTAIN = 0;
    int ACCESS_DENIED = 1;

    public boolean supports(ConfigAttribute configAttribute) {
        return true;
    }

    public int vote(Authentication authentication, Object o, Collection attributes) {
        System.out.println(o.getClass());
        System.out.println(o);
        for (Object attribute : attributes) {
            System.out.println(attribute.getClass());
            System.out.println(attribute);
        }
        return ACCESS_GRANTED;
    }

    public boolean supports(Class aClass) {
        return true;
    }
}
