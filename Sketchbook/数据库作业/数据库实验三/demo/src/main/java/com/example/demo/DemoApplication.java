// docker run -p 3306:3306 --name mysql -e MYSQL_ROOT_PASSWORD=password -e MYSQL_DATABASE=database -d mysql:latest
package com.example.demo;

import org.springframework.boot.SpringApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class DemoApplication {

    private static void withoutJdbcTemplate(DataSource dataSource) throws SQLException {
        Connection connection = dataSource.getConnection();
        Statement statement = connection.createStatement();
        ResultSet resultSet = statement.executeQuery("SELECT * FROM  NUMS");
        while (resultSet.next()) {
            String str = "[ " + resultSet.toString() + " ]";
            str += "[ x: " + resultSet.getInt(1) + ", ";
            str += "y: " + resultSet.getInt(2) + " ]";
            System.out.println(str);
        }
        connection.close();
    }

    private static void simpleDataSourceWithoutJdbcTemplate(ApplicationContext context) throws SQLException {
        DataSource dataSource = context.getBean("dataSource", DataSource.class);
        withoutJdbcTemplate(dataSource);
    }

    private static void dbcpDataSourceWithoutJdbcTemplate(ApplicationContext context) throws SQLException {
        DataSource dataSource = context.getBean("dbcpDataSource", DataSource.class);
        withoutJdbcTemplate(dataSource);
    }

    private static void printTwoNumbers(List<TwoNumber> twoNumbers) {
        for (TwoNumber twoNumber : twoNumbers) {
            System.out.println(twoNumber);
        }
    }

    private static void simpleDataSourceWithJdbcTemplate(ApplicationContext context) throws SQLException {
        JdbcTemplate jdbcTemplate = context.getBean("jdbcTemplate", JdbcTemplate.class);
        jdbcTemplate.execute("DROP TABLE IF EXISTS NUMS");
        jdbcTemplate.execute("CREATE TABLE NUMS (X INT, Y INT)");

        jdbcTemplate.execute("INSERT INTO NUMS VALUES (1, 1)");
        jdbcTemplate.execute("INSERT INTO NUMS VALUES (2, 2)");
        jdbcTemplate.execute("INSERT INTO NUMS VALUES (3, 3)");

        List<TwoNumber> twoNumbers = jdbcTemplate.query("SELECT * FROM NUMS", new TwoNumberRowMapper());
        printTwoNumbers(twoNumbers);

        jdbcTemplate.update("DELETE FROM NUMS WHERE X = 1");
        printTwoNumbers(twoNumbers);

        jdbcTemplate.update("UPDATE NUMS SET X = X + 10");
        twoNumbers = jdbcTemplate.query("SELECT * FROM NUMS", new TwoNumberRowMapper());
        printTwoNumbers(twoNumbers);
    }

    private static void dbcpDataSourceWithJdbcTemplate(ApplicationContext context) throws SQLException {
        JdbcTemplate jdbcTemplate = context.getBean("dbcpJdbcTemplate", JdbcTemplate.class);
        List<TwoNumber> twoNumbers = jdbcTemplate.query("SELECT * FROM NUMS", new TwoNumberRowMapper());
        printTwoNumbers(twoNumbers);
    }

    private static void connectTwoThousands(DataSource dataSource) throws SQLException {
        // for (Integer i = 0; i < 2000; i++) {
            // dataSource.getConnection().close();
        // }
        ArrayList<Thread> threads = new ArrayList<>(4);
        for (Integer i = 0; i < 4; i++) {
            threads.add(new Thread(new Runnable() {
                @Override
                public void run() {
                    for (Integer j = 0; j < 500; j++) {
                        try {
                            dataSource.getConnection().close();
                        } catch (SQLException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }));
        }
        for (Integer i = 0; i < 4; i++) {
            threads.get(i).start();
        }
        for (Integer i = 0; i < 4; i++) {
            try {
                threads.get(i).join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private static Long memoryUsed(DataSource dataSource) throws SQLException {
        Runtime rt = Runtime.getRuntime();
        for (Integer i = 0; i < 3; i++) rt.gc();
        // long startTotalMemory = rt.totalMemory();
        long startFreeMemory = rt.freeMemory();
        connectTwoThousands(dataSource);
        for (Integer i = 0; i < 3; i++) rt.gc();
        return rt.freeMemory() - startFreeMemory;
    }

    private static Long timeUsed(DataSource dataSource) throws SQLException {
        long startTime = System.currentTimeMillis();
        connectTwoThousands(dataSource);
        long stopTime = System.currentTimeMillis();
        long elapsedTime = stopTime - startTime;
        return elapsedTime;
    }

    private static void compareDataSource(ApplicationContext context) throws SQLException {
        DataSource dataSource = context.getBean("dataSource", DataSource.class);
        DataSource dbcpDataSource = context.getBean("dbcpDataSource", DataSource.class);
        System.out.println("TIME: " + timeUsed(dataSource) * 1.0 / timeUsed(dbcpDataSource));
        System.out.println("Memory Allocated: " + memoryUsed(dataSource) * 1.0 / memoryUsed(dbcpDataSource));
    }

    public static void main(String[] args) throws Exception {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("bean.xml");
        try {
            simpleDataSourceWithJdbcTemplate(context);
            dbcpDataSourceWithJdbcTemplate(context);
            simpleDataSourceWithoutJdbcTemplate(context);
            dbcpDataSourceWithoutJdbcTemplate(context);
            compareDataSource(context);
        } finally {
            context.close();
        }

        SpringApplication.run(DemoApplication.class, args);
    }

}