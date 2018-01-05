package com.example.demo;

import org.springframework.jdbc.core.RowMapper;

import java.sql.ResultSet;
import java.sql.SQLException;

public class TwoNumberRowMapper implements RowMapper {

    public Object mapRow(ResultSet rs, int rowNum) throws SQLException {
        TwoNumber number = new TwoNumber();
        number.x = rs.getInt("X");
        number.y = rs.getInt("Y");
        return number;
    }

}
