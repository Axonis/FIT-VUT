<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

use App\Bug;
use App\Ticket;

Route::get('/', 'HomeController@index');
Route::get('/profile', 'ProfileController@index');
Route::get('/profile/{name}', 'ProfileController@show');
Route::post('/profile', 'ProfileController@store');

Route::get('/admin', 'AdminController@index');
Route::get('/admin/{name}', 'AdminController@show');
Route::post('/admin/', 'AdminController@store');

Route::get('/bugs', 'BugController@index');
Route::get('/bugs/create', 'BugController@create_bug');
Route::post('/bugs/create', 'BugController@store_bug');
Route::get('/bugs/{id_bug}', 'BugController@show');
Route::post('/bugs/', 'BugController@store');


Route::get('/tickets', 'TicketController@index');
Route::get('/tickets/create', 'TicketController@create');
Route::get('/tickets/{id_ticket}', 'TicketController@show');
Route::post('/tickets/', 'TicketController@store_ticket');
Route::post('/tickets/create', 'TicketController@store');

Route::get('/patches', 'PatchController@index');
Route::get('/patches/create', 'PatchController@create_show');
Route::get('/patches/{id_bug}', 'PatchController@show');
Route::post('/patches/', 'PatchController@store');
Route::post('/patches/create', 'PatchController@create_store');

Route::get('/tests', 'TestController@index');
Route::get('/tests/create', 'TestController@create_show');
Route::get('/tests/{id_test}', 'TestController@show');
Route::post('/tests/', 'TestController@store');
Route::post('/tests/create', 'TestController@create_store');

Route::get('/modules', 'ModuleController@index');
Route::get('/modules/create', 'ModuleController@create_show');
Route::get('/modules/{id}', 'ModuleController@show');
Route::post('/modules/', 'ModuleController@store');
Route::post('/modules/create', 'ModuleController@create_store');

Auth::routes();

Route::get('/home', 'HomeController@index')->name('home');
