select c1 from t4 where c1 > '20150305123012';
select c1 from t4 where c1 = '20150305123012';
select c1 from t4 where c1 > '00000101000000';
select c1 from t4 where c1 < '00000101000000';
select c1 from t4 where c1 > '99991231235959';
select c1 from t4 where c1 < '99991231235959';
select c1 from t4 where c2 > '20150305123012';
select c1 from t4 where c2 = '20150305123012';
select c1 from t4 where c2 > '00000102080000';
select c1 from t4 where c2 < '00000102080000';
select c1 from t4 where c2 > '99991230235959';
select c1 from t4 where c2 < '99991230235959';
select c1 from t4 where c3 > '2012-01-01';
select c1 from t4 where c3 = '2016-07-27';
select c1 from t4 where c3 > '1000-01-01';
select c1 from t4 where c3 < '1000-01-01';
select c1 from t4 where c3 > '9999-12-31';
select c1 from t4 where c3 < '9999-12-31';
select c1 from t4 where c4 = '12:01:01';
select c1 from t4 where c4 < '838:59:59';
select c1 from t4 where c4 > '838:59:59';
select c1 from t4 where c4 > '00:00:00';
select c1 from t4 where c4 < '00:00:00';
select c1 from t4 where c5 = 2016;
#year类型query range不做抽取
select c1 from t4 where c5 > 1970;
select c1 from t4 where c5 < 1970;
select c1 from t4 where c5 > 2155;
select c1 from t4 where c5 < 2155;
